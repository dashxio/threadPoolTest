cmake_minimum_required(VERSION 3.14)
project(testThreadPool)

set(CMAKE_CXX_STANDARD 17)

file(REMOVE_RECURSE "build")
file(GLOB_RECURSE SRC *.cpp)

add_executable(testThreadPool ${SRC})
