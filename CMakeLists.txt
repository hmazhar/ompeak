cmake_minimum_required(VERSION 2.8)
project(ompeak)

find_package("OpenMP" REQUIRED)

# Add the OpenMP-specific compiler and linker flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}  -msse4.2")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}  -msse4.2")

include_directories(${OPENCL_INCLUDE_DIR})

ADD_EXECUTABLE(ompeak src/main.cpp)		
TARGET_LINK_LIBRARIES(ompeak)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")