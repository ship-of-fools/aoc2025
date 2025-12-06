cmake_minimum_required(VERSION 3.21)
cmake_policy(VERSION 3.21)

set(CMAKE_CXX_STANDARD 17)

set(name "Day06")

project(${name} LANGUAGES CXX)

add_executable(${name} main.cpp)

# Strip off the inclusive O3 from Release flags and set custom flags
string(REGEX REPLACE "\-O3" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
# Set custom optimisations incl LTO, and set C++23
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23 -static -O3 -march=native -mtune=native -flto")
# Ensure static linking (and LTO)
set_target_properties(${name} PROPERTIES LINK_SEARCH_START_STATIC ON)
set_target_properties(${name} PROPERTIES LINK_SEARCH_END_STATIC ON)
target_link_options(${name} PRIVATE -static -flto)
