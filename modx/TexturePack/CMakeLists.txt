cmake_minimum_required(VERSION 3.22.1)

project("TexturePack")

add_subdirectory("../Dobby", "../Dobby/Build")
add_subdirectory("../BNM-Android-Hook" "../BNM-Android-Hook/build")

target_link_libraries(BNM
        PUBLIC
        dobby_static
)

add_library(${CMAKE_PROJECT_NAME} SHARED
        includes/miniz.c
        includes/lodepng.cpp

        src/TexturePack.cpp
        src/PNGDecoder.cpp
        src/Texture2DConverter.cpp
        src/LoadTexture2D.cpp
)


target_include_directories(${CMAKE_PROJECT_NAME}
        PUBLIC
        includes
)

target_link_libraries(${CMAKE_PROJECT_NAME}
        PUBLIC
        android
        GLESv2
        BNM
        log)


