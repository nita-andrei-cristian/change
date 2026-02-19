cmake_minimum_required(VERSION 3.16)
project(change CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(change ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
