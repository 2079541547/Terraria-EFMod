cmake_minimum_required(VERSION 3.22.1)

project("Overclock")


add_library(Overclock SHARED
            efmod.cpp
)

target_include_directories(Overclock
        PUBLIC
        ../efmod-api
        ../tefmod-api
)