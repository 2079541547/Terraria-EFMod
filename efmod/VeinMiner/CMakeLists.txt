cmake_minimum_required(VERSION 3.31)
project(VeinMiner)

set(CMAKE_CXX_STANDARD 17)

add_library(VeinMiner SHARED
        src/efmod.cpp
        src/tile_data.cpp
        src/vein_miner.cpp)

target_include_directories(VeinMiner
        PUBLIC
        ../efmod-api
        ../tefmod-api
        includes
)