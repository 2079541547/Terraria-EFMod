cmake_minimum_required(VERSION 3.31)
project(InfiniteLegion)

set(CMAKE_CXX_STANDARD 17)

add_library(InfiniteLegion SHARED
        efmod.cpp)

target_include_directories(InfiniteLegion
        PUBLIC
        ../efmod-api
        ../tefmod-api
)