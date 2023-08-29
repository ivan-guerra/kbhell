#include "kbhit/linux_kbhit.hpp"

#include <sys/time.h>
#include <termios.h>
#include <unistd.h>

#include <cstdio>

namespace kbhell {

int _kbhit(int timeout_sec) {
    static bool initialized = false;

    if (!initialized) {
        struct termios ttystate = {};
        tcgetattr(STDIN_FILENO, &ttystate);
        ttystate.c_lflag &= (~ICANON & ~ECHO);  // Not display character
        ttystate.c_cc[VMIN] = 1;
        tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

        initialized = true;
    }

    struct timeval tv = {.tv_sec = timeout_sec, .tv_usec = 0};
    fd_set fds = {};
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);

    return FD_ISSET(STDIN_FILENO, &fds);
}

}  // namespace kbhell
