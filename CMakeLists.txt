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

aux_source_directory(test TESTS)
add_executable(tests ${TESTS})
target_link_libraries(tests
    PRIVATE
    # BigInteger
    gtest_main
)

add_test(test_all tests)