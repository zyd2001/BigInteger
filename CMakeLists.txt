cmake_minimum_required(VERSION 3.0.0)
project(BigInteger VERSION 0.1.0)

set(CMAKE_CXX_STANDARD_REQUIRED 11)

aux_source_directory(src SRCS)
add_library(BigInteger SHARED ${SRCS})
target_include_directories(BigInteger
    PUBLIC
    include/
)

# test
add_subdirectory(googletest)
enable_testing()

add_executable(constructorTest test/constructor.cpp)
add_executable(toStringTest test/toString.cpp)
target_link_libraries(constructorTest BigInteger gtest_main)
target_link_libraries(toStringTest BigInteger gtest_main)

add_test(constructorTest constructorTest)
add_test(toStringTest toStringTest)
