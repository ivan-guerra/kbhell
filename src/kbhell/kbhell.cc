#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "wav_player/wav_player.hpp"

#ifdef __linux__
#include "kbhit/linux_kbhit.hpp"
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <conio.h>
#include <windows.h>
#endif

void PrintUsage() {
    std::cout << "usage: kbhell WAV_FILE" << std::endl;
    std::cout << "play WAV_FILE everytime a key is pressed (press ESC to stop)"
              << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        PrintUsage();
        std::exit(EXIT_FAILURE);
    }

    try {
        kbhell::WavPlayer player(argv[1]);
        bool exit = false;
        int rc = 0;
        const char kEsc = static_cast<char>(27); /* ASCII ESC character */
        while (!exit) {
#ifdef __linux__
            rc = kbhell::_kbhit();
            if (-1 == rc) {
                std::cerr << "error: failed to poll keyboard" << std::endl;
                std::exit(EXIT_FAILURE);
            } else if (rc > 0) {
                if (std::fgetc(stdin) == kEsc) {
                    exit = true;
                } else {
                    player.Play();
                }
            }
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
            rc = _kbhit();
            if (rc) {
                if (std::fgetc(stdin) == kEsc) {
                    exit = true;
                } else {
                    player.Play();
                }
            }
#endif
        }
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::exit(EXIT_SUCCESS);
}
