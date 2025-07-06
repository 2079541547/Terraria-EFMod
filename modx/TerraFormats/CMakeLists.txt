cmake_minimum_required(VERSION 3.22.1)

project("TerraFormats")

add_subdirectory("../Dobby", "../Dobby/Build")
add_subdirectory("../BNM-Android-Hook" "../BNM-Android-Hook/build")

target_link_libraries(BNM
        PUBLIC
        dobby_static
)

add_library(${CMAKE_PROJECT_NAME} SHARED
        includes/miniz.c

        # src/TerraFormats.cpp

        src/LanguagePacks.cpp
        src/Item.cpp
        src/LanguageManager.cpp

        src/Modx.cpp

        ${${CMAKE_SOURCE_DIR}/luajit/${ANDROID_ABI}/libluajit.a}
)


target_include_directories(${CMAKE_PROJECT_NAME}
        PUBLIC
        includes
)

target_link_libraries(${CMAKE_PROJECT_NAME}
        PUBLIC
        android
        BNM
        log)


