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
option(BUILD_GMOCK off)
add_subdirectory(googletest)
enable_testing()

add_executable(constructorTest test/constructor.cpp)
add_executable(toStringTest test/toString.cpp)
add_executable(compareTest test/compare.cpp)
add_executable(shiftTest test/shift.cpp)
add_executable(addsubTest test/addsub.cpp)
add_executable(logicTest test/logic.cpp)
add_executable(mulTest test/mul.cpp)
add_executable(divTest test/div.cpp)
target_link_libraries(constructorTest BigInteger gtest_main)
target_link_libraries(toStringTest BigInteger gtest_main)
target_link_libraries(compareTest BigInteger gtest_main)
target_link_libraries(shiftTest BigInteger gtest_main)
target_link_libraries(addsubTest BigInteger gtest_main)
target_link_libraries(logicTest BigInteger gtest_main)
target_link_libraries(mulTest BigInteger gtest_main)
target_link_libraries(divTest BigInteger gtest_main)

add_test(constructorTest constructorTest)
add_test(toStringTest toStringTest)
add_test(compareTest compareTest)
add_test(shiftTest shiftTest)
add_test(addsubTest addsubTest)
add_test(logicTest logicTest)
add_test(mulTest mulTest)
add_test(divTest divTest)
