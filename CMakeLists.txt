cmake_minimum_required(VERSION 3.0.0)
project(BigInteger VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

aux_source_directory(src SRCS)
add_library(BigInteger SHARED ${SRCS})
target_include_directories(BigInteger
    PUBLIC
    include/
)

# test
option(BIGINTEGER_TEST "Enable BigInteger Test")
if(BIGINTEGER_TEST)
    enable_testing()
    add_subdirectory(test)
endif(BIGINTEGER_TEST)
