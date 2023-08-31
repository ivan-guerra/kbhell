#include "kbhell/event_loop.hpp"

#include <cstdio>
#include <iostream>

#ifdef __linux__
#include "kbhit/linux_kbhit.hpp"

int kbhell::RunEventLoop(WavPlayer& player) {
    bool exit = false;
    int rc = 0;
    const char kEsc = static_cast<char>(27); /* ASCII ESC character */
    while (!exit) {
        rc = kbhell::_kbhit();
        if (-1 == rc) {
            std::cerr << "error: failed to poll keyboard" << std::endl;
            return -1;
        } else if (rc > 0) {
            if (std::fgetc(stdin) == kEsc) {
                exit = true;
            } else {
                player.Play();
            }
        }
    }
    return 0;
}

#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <conio.h>
#include <windows.h>

int kbhell::RunEventLoop(WavPlayer& player) {
    bool exit = false;
    int rc = 0;
    const char kEsc = static_cast<char>(27); /* ASCII ESC character */
    while (!exit) {
        rc = _kbhit();
        if (rc) {
            if (std::fgetc(stdin) == kEsc) {
                exit = true;
            } else {
                player.Play();
            }
        }
    }
    return 0;
}

#endif
