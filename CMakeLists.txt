cmake_minimum_required(VERSION 3.27)

project(kbhell DESCRIPTION "keyboard sound augmentation")

# Append the SDL2 Find* script paths to CMAKE_MODULE_PATH.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(KBHELL_BIN_DIR "${CMAKE_SOURCE_DIR}/bin"
    CACHE STRING   "${PROJECT_NAME} binary directory.")
set(KBHELL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include"
    CACHE STRING       "${PROJECT_NAME} include directory.")

include(FetchContent)

FetchContent_Declare(googletest
                     GIT_REPOSITORY https://github.com/google/googletest.git
                     GIT_TAG release-1.12.1)
FetchContent_MakeAvailable(googletest)

add_subdirectory(docs)
add_subdirectory(src)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(PACKAGE_TESTS "Build Unit Tests" OFF)
    if(PACKAGE_TESTS)
        enable_testing()
        include(GoogleTest)
        add_subdirectory(tests)
    endif()
endif()
