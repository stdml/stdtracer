INCLUDE(${CMAKE_SOURCE_DIR}/cmake/gtest.cmake)

FUNCTION(ADD_UNIT_TEST target)
    ADD_EXECUTABLE(${target} ${ARGN} tests/main.cpp)
    TARGET_USE_GTEST(${target})
    TARGET_INCLUDE_DIRECTORIES(${target}
                               PRIVATE ${CMAKE_SOURCE_DIR}/include)
    TARGET_INCLUDE_DIRECTORIES(${target}
                               PRIVATE ${CMAKE_SOURCE_DIR}/tests/include)
    IF(HAVE_CUDA)
        TARGET_LINK_LIBRARIES(${target} cudart)
    ENDIF()
    ADD_TEST(NAME ${target} COMMAND ${target})
ENDFUNCTION()

FUNCTION(ADD_UNIT_TESTS)
    FOREACH(t ${ARGN})
        GET_FILENAME_COMPONENT(name ${t} NAME_WE)
        STRING(REPLACE "_" "-" name ${name})
        ADD_UNIT_TEST(${name} ${t})
    ENDFOREACH()
ENDFUNCTION()

FILE(GLOB tests tests/test_*.cpp)

ADD_UNIT_TESTS(${tests})
