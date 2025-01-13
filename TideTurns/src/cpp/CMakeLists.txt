cmake_minimum_required(VERSION 3.22.1)

project("efmod")

add_subdirectory(BNM-Android-master)

add_library(${CMAKE_PROJECT_NAME} SHARED
    LuckyDamage.cpp
    efmod.cpp
    ItemRoulette.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    android BNM log)
