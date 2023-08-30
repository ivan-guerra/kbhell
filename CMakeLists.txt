cmake_minimum_required(VERSION 3.27)

project(kbhell DESCRIPTION "keyboard sound augmentation")

set(KBHELL_BIN_DIR "${CMAKE_SOURCE_DIR}/bin"
    CACHE STRING   "${PROJECT_NAME} binary directory.")
set(KBHELL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include"
    CACHE STRING       "${PROJECT_NAME} include directory.")

include(FetchContent)

FetchContent_Declare(googletest
                     GIT_REPOSITORY https://github.com/google/googletest.git
                     GIT_TAG release-1.12.1)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(SDL
                     GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
                     GIT_TAG release-2.28.2)
FetchContent_MakeAvailable(SDL)

set(SDL2MIXER_VENDORED ON CACHE INTERNAL "use vendored (not system) libs")
FetchContent_Declare(SDL_mixer
                     GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
                     GIT_TAG SDL2)
FetchContent_MakeAvailable(SDL_mixer)

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
