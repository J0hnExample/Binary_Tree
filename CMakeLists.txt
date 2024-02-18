cmake_minimum_required(VERSION 3.10)

project(A3)

set(CMAKE_CXX_STANDARD "20")

set(SOURCE_FILES main.cpp payload_v2.cpp)

add_executable(treemap ${SOURCE_FILES})
