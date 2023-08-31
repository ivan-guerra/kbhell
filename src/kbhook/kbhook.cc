#include "kbhook/kbhook.hpp"

#include <windows.h>

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode < 0) { /* do not proces message */
        return CallNextHookEx(nullptr, nCode, wParam, lParam);
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}
