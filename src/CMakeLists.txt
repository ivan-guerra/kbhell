cmake_minimum_required(VERSION 3.27)

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

add_subdirectory(kbhell)
add_subdirectory(wav_player)

if(UNIX)
    add_subdirectory(kbhit)
endif(UNIX)
