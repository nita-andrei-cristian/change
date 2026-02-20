cmake_minimum_required(VERSION 3.16)
project(change)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for lsp

add_subdirectory(${CMAKE_SOURCE_DIR}/llama.cpp)

#add_library(llama STATIC ${CMAKE_SOURCE_DIR}/llama.cpp)
add_library(ai SHARED ${CMAKE_SOURCE_DIR}/include/ai.cpp)
add_library(brain SHARED ${CMAKE_SOURCE_DIR}/include/brain.cpp)

target_include_directories(ai PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(brain PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(change ${CMAKE_SOURCE_DIR}/src/main.cpp)

target_link_libraries(change PRIVATE ai brain llama ggml)
