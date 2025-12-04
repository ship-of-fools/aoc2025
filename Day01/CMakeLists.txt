cmake_minimum_required(VERSION 3.21)
cmake_policy(VERSION 3.21)

set(CMAKE_CXX_STANDARD 17)

set(name "Day01")

project(${name} LANGUAGES CXX)

add_executable(${name} main.cpp)
