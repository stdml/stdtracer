FUNCTION(ADD_TRACE_EXAMPLE target)
    ADD_EXECUTABLE(${target} ${ARGN})
    TARGET_INCLUDE_DIRECTORIES(${target} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    TARGET_LINK_LIBRARIES(${target} stdtracer)
ENDFUNCTION()

ADD_TRACE_EXAMPLE(examples-1 examples/example_1.cpp)
ADD_TRACE_EXAMPLE(examples-2 examples/example_2.cpp)
