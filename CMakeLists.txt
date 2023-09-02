cmake_minimum_required(VERSION 3.27)

project(kbhell DESCRIPTION "keyboard sound augmentation")

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

FetchContent_Declare(SDL
                     GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
                     GIT_TAG release-2.28.2)
FetchContent_MakeAvailable(SDL)

FetchContent_Declare(SDL_mixer
                     GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
                     GIT_TAG SDL2)
include(SDLMixerConfig)
FetchContent_MakeAvailable(SDL_mixer)

if(UNIX)
    set(COMMON_FLAGS
        -std=c++2a
    )

    set(WARNING_FLAGS
        -Wall
        -Wextra
        -Werror
        -pedantic
    )

    set(DEBUG_FLAGS
        ${COMMON_FLAGS}
        ${WARNING_FLAGS}
        -O0
        -g3
        -ggdb
        -fno-omit-frame-pointer
        -fsanitize=address
    )

    set(RELEASE_FLAGS
        ${COMMON_FLAGS}
        -O2
    )
elseif(WIN32)
    set(COMMON_FLAGS
        /std:c++20
    )

    set(WARNING_FLAGS
        /Wall
    )

    set(DEBUG_FLAGS
        ${COMMON_FLAGS}
        ${WARNING_FLAGS}
        /DEBUG:FULL
    )

    set(RELEASE_FLAGS
        ${COMMON_FLAGS}
        /O2
    )
endif()

add_compile_options(
    "$<$<CONFIG:Release>:${RELEASE_FLAGS}>"
    "$<$<CONFIG:Debug>:${DEBUG_FLAGS}>"
)

if(UNIX)
    add_link_options(
        "$<$<CONFIG:Debug>:-fsanitize=address>"
    )
endif(UNIX)

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
