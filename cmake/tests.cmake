INCLUDE(ExternalProject)

SET(GTEST_GIT_URL
    https://github.com/google/googletest.git
    CACHE
    STRING
    "URL for clone gtest")

EXTERNALPROJECT_ADD(libgtest-dev
                    GIT_REPOSITORY
                    ${GTEST_GIT_URL}
                    CMAKE_ARGS
                    -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/3rdparty
                    -DCMAKE_CXX_FLAGS=-std=c++11
                    -Dgtest_disable_pthreads=1
                    -DBUILD_GMOCK=0)

LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/3rdparty/lib)

ADD_EXECUTABLE(test-1 tests/test_1.cpp)
ADD_DEPENDENCIES(test-1 libgtest-dev)
TARGET_INCLUDE_DIRECTORIES(test-1
                           PRIVATE
                           ${CMAKE_SOURCE_DIR}/include
                           ${CMAKE_SOURCE_DIR}/3rdparty/include)
TARGET_LINK_LIBRARIES(test-1 stdtracer gtest)

ADD_TEST(NAME test-1 COMMAND test-1)
