cmake_minimum_required(VERSION 3.27)

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

project(wav_player
    DESCRIPTION "a WAV file player using the SDL2 library"
    LANGUAGES   CXX
)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PRIVATE wav_player.cc
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${KBHELL_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE SDL2::Main
    PRIVATE SDL2::Mixer
)
