cmake_minimum_required(VERSION 3.27)

add_executable(${CMAKE_PROJECT_NAME})

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE kbhell.cc
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    PRIVATE kbhit
    PRIVATE wav_player
)

install(TARGETS ${CMAKE_PROJECT_NAME}
    RUNTIME DESTINATION ${KBHELL_BIN_DIR}
)
