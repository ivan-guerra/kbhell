#include "linux_kbhit/linux_kbhit.hpp"

#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>

namespace kbhell {

int _kbhit() {
    static const int kStdin = 0;
    static bool initialized = false;

    if (!initialized) { /* Use termios to turn off line buffering */
        termios term = {};
        tcgetattr(kStdin, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(kStdin, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytes_waiting = 0;
    return (-1 == ioctl(kStdin, FIONREAD, &bytes_waiting)) ? -1 : bytes_waiting;
}

}  // namespace kbhell
