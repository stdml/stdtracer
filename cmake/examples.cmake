FIND_PACKAGE(Threads)

FUNCTION(ADD_TRACE_EXAMPLE target)
    ADD_EXECUTABLE(${target} ${ARGN})
    TARGET_INCLUDE_DIRECTORIES(${target} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    TARGET_LINK_LIBRARIES(${target} stdtracer)
    TARGET_LINK_LIBRARIES(${target} Threads::Threads)
ENDFUNCTION()

FUNCTION(ADD_TRACE_DISABLED_EXAMPLE target)
    ADD_EXECUTABLE(${target} ${ARGN})
    TARGET_INCLUDE_DIRECTORIES(${target} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    TARGET_COMPILE_DEFINITIONS(${target} PRIVATE -DSTD_TRACER_DISABLE)
    TARGET_LINK_LIBRARIES(${target} Threads::Threads)
ENDFUNCTION()

FILE(GLOB examples examples/example_*.cpp)
FOREACH(src ${examples})
    GET_FILENAME_COMPONENT(name ${src} NAME_WE)
    STRING(REPLACE "_" "-" name ${name})
    ADD_TRACE_EXAMPLE(${name} ${src})
    ADD_TRACE_DISABLED_EXAMPLE(${name}-disabled ${src})
ENDFOREACH()

FUNCTION(ADD_ADVANCED_TRACE_EXAMPLE target)
    ADD_EXECUTABLE(${target} ${ARGN})
    TARGET_INCLUDE_DIRECTORIES(${target} PRIVATE ${CMAKE_SOURCE_DIR}/include)
ENDFUNCTION()

ADD_ADVANCED_TRACE_EXAMPLE(example-multi examples/multi/example_multi.cpp
                           examples/multi/trace.cpp)
