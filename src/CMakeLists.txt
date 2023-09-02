cmake_minimum_required(VERSION 3.27)

add_subdirectory(kbhell)
add_subdirectory(wav_player)
if(WIN32)
    # Global Windows hooks have to be in a dedicated DLL. kbhook is the DLL
    # containing the keyboard hook for the Windows kbhell driver.
    add_subdirectory(kbhook)
endif(WIN32)
