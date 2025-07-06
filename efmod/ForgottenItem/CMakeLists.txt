cmake_minimum_required(VERSION 3.22.1)
project(ForgottenItem)

set(CMAKE_CXX_STANDARD 17)

add_library(ForgottenItem SHARED
        src/efmod.cpp
        src/item_id.cpp
        src/recipe.cpp
        src/item.cpp
        src/prefix_legacy.cpp
        src/item_drop_database.cpp
)

target_include_directories(ForgottenItem
        PUBLIC
        includes
        ../tefmod-api
        ../efmod-api
)
