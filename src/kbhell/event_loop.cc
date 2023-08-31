#include "kbhell/event_loop.hpp"

#include <cstdio>
#include <iostream>
#include <stdexcept>

#ifdef __linux__
#include "kbhit/linux_kbhit.hpp"

void kbhell::RunEventLoop(WavPlayer& player) {
    bool exit = false;
    int rc = 0;
    const char kEsc = static_cast<char>(27); /* ASCII ESC character */
    while (!exit) {
        rc = kbhell::_kbhit();
        if (-1 == rc) {
            throw std::runtime_error("failed to poll keyboard");
        } else if (rc > 0) {
            if (std::fgetc(stdin) == kEsc) {
                exit = true;
            } else {
                player.Play();
            }
        }
    }
}

#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <conio.h>
#include <windows.h>

#include <cstdint>
#include <string>

/* https://stackoverflow.com/questions/1387064/how-to-get-the-error-message-from-the-error-code-returned-by-getlasterror
 */
static std::string GetLastErrorAsString() {
    /* get the error message ID, if any */
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0) {
        return std::string(); /* no error message has been recorded */
    }

    LPSTR messageBuffer = nullptr;

    /* Ask Win32 to give us the string version of that message ID.
     * The parameters we pass in, tell Win32 to create the buffer that holds
     * the message for us (because we don't yet know how long the message
     * string will be).
     */
    size_t size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&messageBuffer, 0, NULL);

    /* copy the error message into a std::string */
    std::string message(messageBuffer, size);

    /* free the Win32's string's buffer */
    LocalFree(messageBuffer);

    return message;
}

void kbhell::RunEventLoop(WavPlayer& player) {
    /* Setup a global keyboard hook. The kbhook.dll DLL contains the keyboard
     * hook function implementation that gets called on each keypress. */
    HINSTANCE kbhook_dll = LoadLibrary(TEXT("kbhook.dll"));
    if (!kbhook_dll) {
        throw std::runtime_error(GetLastErrorAsString());
    }
    HOOKPROC kbhook_hook = (HOOKPROC)GetProcAddress(kbhook_dll, "KeyboardProc");
    if (!kbhook_hook) {
        FreeLibrary(kbhook_dll);
        throw std::runtime_error(GetLastErrorAsString());
    }
    HHOOK kbhook_handle =
        SetWindowsHookEx(WH_KEYBOARD, kbhook_hook, kbhook_dll, 0);
    if (!kbhook_handle) {
        FreeLibrary(kbhook_dll);
        throw std::runtime_error(GetLastErrorAsString());
    }

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

    if (kbhook_handle && !UnhookWindowsHookEx(kbhook_handle)) {
        throw std::runtime_error(GetLastErrorAsString());
    }
    if (kbhook_dll && !FreeLibrary(kbhook_dll)) {
        throw std::runtime_error(GetLastErrorAsString());
    }
}

#endif
