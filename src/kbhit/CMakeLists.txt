cmake_minimum_required(VERSION 3.27)

project(kbhit
    DESCRIPTION "cross-platform key press detection functions"
    LANGUAGES   CXX
)

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
    PRIVATE linux_kbhit.cc
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${KBHELL_INCLUDE_DIR}
)
