cmake_minimum_required(VERSION 3.27)

project(kbhook
    DESCRIPTION "a windows global hook for detecting keypresses"
    LANGUAGES   CXX
)

add_library(${PROJECT_NAME} SHARED)

target_sources(${PROJECT_NAME}
    PRIVATE kbhook.cc
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${KBHELL_INCLUDE_DIR}
)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${KBHELL_BIN_DIR}
)
