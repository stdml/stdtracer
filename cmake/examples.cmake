ADD_EXECUTABLE(example-1 examples/example_1.cpp)
TARGET_INCLUDE_DIRECTORIES(example-1 PRIVATE ${CMAKE_SOURCE_DIR}/include)
TARGET_LINK_LIBRARIES(example-1 stdtracer)

ADD_EXECUTABLE(example-2 examples/example_2.cpp)
TARGET_INCLUDE_DIRECTORIES(example-2 PRIVATE ${CMAKE_SOURCE_DIR}/include)
TARGET_LINK_LIBRARIES(example-2 stdtracer)
