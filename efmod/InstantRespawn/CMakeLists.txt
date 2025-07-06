cmake_minimum_required(VERSION 3.22.1)
project(InstantRespawn)

set(CMAKE_CXX_STANDARD 17)

add_library(InstantRespawn SHARED
        efmod.cpp)

target_include_directories(InstantRespawn
        PUBLIC
        ../efmod-api
        ../tefmod-api
)
