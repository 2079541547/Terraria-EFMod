cmake_minimum_required(VERSION 3.22.1)

project("Overclock")

add_subdirectory(BNM-Android-master)

add_library(Overclock SHARED
            efmod.cpp
)

target_link_libraries(Overclock
        android
        BNM
        log)
