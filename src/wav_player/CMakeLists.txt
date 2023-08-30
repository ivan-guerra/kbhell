cmake_minimum_required(VERSION 3.27)

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
   PUBLIC SDL2::SDL2
   PUBLIC SDL2_mixer::SDL2_mixer
)
