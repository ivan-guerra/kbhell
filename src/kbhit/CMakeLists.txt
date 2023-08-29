cmake_minimum_required(VERSION 3.27)

project(kbhit
    DESCRIPTION "cross-platform key press detection functions"
    LANGUAGES   CXX
)

add_library(${PROJECT_NAME} STATIC)

if (UNIX)
    target_sources(${PROJECT_NAME}
        PRIVATE linux_kbhit.cc
    )
endif(UNIX)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${KBHELL_INCLUDE_DIR}
)
