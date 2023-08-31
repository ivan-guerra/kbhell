cmake_minimum_required(VERSION 3.27)

add_executable(${CMAKE_PROJECT_NAME})

target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE kbhell.cc
    PRIVATE event_loop.cc
)

if(UNIX)
    target_link_libraries(${CMAKE_PROJECT_NAME}
        PRIVATE kbhit
        PRIVATE wav_player
    )
elseif(WIN32)
    target_link_libraries(${CMAKE_PROJECT_NAME}
        PRIVATE wav_player
    )
endif()

install(TARGETS ${CMAKE_PROJECT_NAME}
    RUNTIME DESTINATION ${KBHELL_BIN_DIR}
)
