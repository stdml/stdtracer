FUNCTION(ADD_TRACE_EXAMPLE target)
    ADD_EXECUTABLE(${target} ${ARGN})
    TARGET_INCLUDE_DIRECTORIES(${target} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    TARGET_LINK_LIBRARIES(${target} stdtracer)
ENDFUNCTION()

FUNCTION(ADD_TRACE_DISABLED_EXAMPLE target)
    ADD_EXECUTABLE(${target} ${ARGN})
    TARGET_INCLUDE_DIRECTORIES(${target} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    TARGET_COMPILE_DEFINITIONS(${target} PRIVATE -DSTD_TRACER_DISABLE)
ENDFUNCTION()

ADD_TRACE_EXAMPLE(examples-1 examples/example_1.cpp)
ADD_TRACE_EXAMPLE(examples-2 examples/example_2.cpp)

ADD_TRACE_DISABLED_EXAMPLE(examples-1-disable examples/example_1.cpp)
ADD_TRACE_DISABLED_EXAMPLE(examples-2-disable examples/example_2.cpp)

FUNCTION(ADD_ADVANCED_TRACE_EXAMPLE target)
    ADD_EXECUTABLE(${target} ${ARGN})
    TARGET_INCLUDE_DIRECTORIES(${target} PRIVATE ${CMAKE_SOURCE_DIR}/include)
ENDFUNCTION()

ADD_ADVANCED_TRACE_EXAMPLE(example-multi examples/multi/example_multi.cpp
                           examples/multi/trace.cpp)
