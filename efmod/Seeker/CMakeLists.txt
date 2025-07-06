cmake_minimum_required(VERSION 3.22.1)

project("Seeker")

add_library(Seeker SHARED
            efmod.cpp
)


target_include_directories(Seeker
        PUBLIC
        ../efmod-api
        ../tefmod-api
)