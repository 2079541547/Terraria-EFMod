cmake_minimum_required(VERSION 3.22.1)

project("Accessorize")

add_library(Accessorize SHARED
            efmod.cpp
)

target_include_directories(Accessorize
        PUBLIC
        ../efmod-api
        ../tefmod-api
)