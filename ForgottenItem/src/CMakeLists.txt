cmake_minimum_required(VERSION 3.22.1)

project("forgottenitem")

add_subdirectory(BNM-Android-master)
add_subdirectory(Dobby)

target_link_libraries(BNM
    PUBLIC
        Dobby
)

add_library(forgottenitem SHARED
        src/efmod.cpp
        src/ItemID.cpp
        src/modx.cpp
        src/Recipe.cpp
        src/Item.cpp
)

target_include_directories(forgottenitem
        PRIVATE
        includes/forgottenitem
)

target_link_libraries(forgottenitem
        android
        BNM
        log)
