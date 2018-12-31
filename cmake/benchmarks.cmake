INCLUDE(ExternalProject)

SET(GBENCH_GIT_URL https://github.com/google/benchmark.git
    CACHE STRING "URL for clone google benchmark")

SET(PREFIX ${CMAKE_SOURCE_DIR}/3rdparty)

EXTERNALPROJECT_ADD(gbench-repo
                    GIT_REPOSITORY
                    ${GBENCH_GIT_URL}
                    PREFIX
                    ${PREFIX}
                    CMAKE_ARGS
                    -DCMAKE_INSTALL_PREFIX=${PREFIX}
                    -DCMAKE_BUILD_TYPE=Release
                    -DCMAKE_CXX_FLAGS=-std=c++11
                    -DBENCHMARK_ENABLE_TESTING=0)

LINK_DIRECTORIES(${PREFIX}/lib)

ADD_CUSTOM_TARGET(benchmarks)

FIND_PACKAGE(Threads REQUIRED)
FUNCTION(ADD_BENCH target)
    ADD_EXECUTABLE(${target} ${ARGN})
    TARGET_INCLUDE_DIRECTORIES(${target} PRIVATE ${PREFIX}/include)
    TARGET_INCLUDE_DIRECTORIES(${target} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    TARGET_LINK_LIBRARIES(${target} benchmark benchmark_main Threads::Threads)
    ADD_DEPENDENCIES(${target} gbench-repo stdtracer)
    ADD_DEPENDENCIES(benchmarks ${target})
ENDFUNCTION()

FILE(GLOB benches tests/bench_*.cpp)
FOREACH(b ${benches})
    GET_FILENAME_COMPONENT(name ${b} NAME_WE)
    STRING(REPLACE "_"
                   "-"
                   name
                   ${name})
    ADD_BENCH(${name} ${b})
ENDFOREACH()
