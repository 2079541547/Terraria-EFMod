cmake_minimum_required(VERSION 3.22.1)

project("Seeker")

add_subdirectory(BNM-Android-master)

add_library(Seeker SHARED
            efmod.cpp
)

target_include_directories(Seeker
        PRIVATE
        includes
)

target_link_libraries(Seeker
        android
        BNM
        log)
