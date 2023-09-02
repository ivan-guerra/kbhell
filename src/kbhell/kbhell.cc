#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "kbhell/event_loop.hpp"
#include "wav_player/wav_player.hpp"

void PrintUsage() {
    std::cout << "usage: kbhell WAV_FILE" << std::endl;
    std::cout << "play WAV_FILE everytime a key is released (press ESC to stop)"
              << std::endl;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        PrintUsage();
        std::exit(EXIT_FAILURE);
    }

    try {
        kbhell::WavPlayer player(argv[1]);
        RunEventLoop(player);
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::exit(EXIT_SUCCESS);
}
