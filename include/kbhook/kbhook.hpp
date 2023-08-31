#ifndef KBHOOK_HPP_
#define KBHOOK_HPP_

#include <windows.h>

extern "C" __declspec(dllexport) LRESULT CALLBACK
    KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

#endif
