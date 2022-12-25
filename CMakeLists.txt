cmake_minimum_required(VERSION 3.22)

project(maltipoo)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(ExternalProject)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(thirdparty)
add_subdirectory(maltipoo)
add_subdirectory(examples)