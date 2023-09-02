#include "kbhell/event_loop.hpp"

#include <cstdio>
#include <iostream>
#include <stdexcept>

#ifdef __linux__
#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include <X11/extensions/record.h>

#include <atomic>

static std::atomic_bool exit_event_loop(false);

/* refer to libxnee */
union XRecordDatum {
    unsigned char type;
    xEvent event;
    xResourceReq req;
    xGenericReply reply;
    xError error;
    xConnSetupPrefix setup;
};

void KeyCallback(XPointer closure, XRecordInterceptData* hook) {
    if (hook->category != XRecordFromServer) {
        ::XRecordFreeData(hook);
        return;
    }

    kbhell::WavPlayer* player = reinterpret_cast<kbhell::WavPlayer*>(closure);
    XRecordDatum* data = reinterpret_cast<XRecordDatum*>(hook->data);

    int event_type = data->type;
    BYTE keycode = data->event.u.u.detail;
    const int kEsc = 9;
    switch (event_type) {
        case KeyRelease:
            if (keycode == kEsc) { /* if ESC is pressed at any time, exit */
                exit_event_loop = true;
            } else {
                player->Play();
            }
            break;
        default:
            break;
    }
    ::XRecordFreeData(hook);
}

void kbhell::RunEventLoop(WavPlayer& player) {
    ::Display* ctrl_disp = XOpenDisplay(nullptr);
    ::Display* data_disp = XOpenDisplay(nullptr);
    if (!ctrl_disp || !data_disp) {
        throw std::runtime_error("unable to open X display");
    }

    int major = 0;
    int minor = 0;
    if (!::XRecordQueryVersion(ctrl_disp, &major, &minor)) {
        throw std::runtime_error(
            "RECORD extension not supported on this X server");
    }

    /* we must set the ctrl_disp to sync mode, or, when we enable the context
     * in data_disp, there will be a fatal X error */
    ::XSynchronize(ctrl_disp, true);

    ::XRecordRange* record_rng = ::XRecordAllocRange();
    if (!record_rng) {
        throw std::runtime_error("could not alloc record range");
    }
    record_rng->device_events.first = KeyPress;
    record_rng->device_events.last = KeyRelease;

    ::XRecordClientSpec record_spec = XRecordAllClients;
    ::XRecordContext record_ctx =
        ::XRecordCreateContext(ctrl_disp, 0, &record_spec, 1, &record_rng, 1);
    if (!record_ctx) {
        throw std::runtime_error("could not create record context");
    }

    if (!::XRecordEnableContextAsync(data_disp, record_ctx, KeyCallback,
                                     reinterpret_cast<::XPointer>(&player))) {
        throw std::runtime_error("could not enable record context");
    }

    while (!exit_event_loop) {
        ::XRecordProcessReplies(data_disp);
    }

    ::XRecordDisableContext(ctrl_disp, record_ctx);
    ::XRecordFreeContext(ctrl_disp, record_ctx);
    ::XFree(record_rng);
    ::XCloseDisplay(data_disp);
    ::XCloseDisplay(ctrl_disp);
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
    /* create a mutex used to sync access to shared memory */
    HANDLE kbhook_mtx = CreateMutex(nullptr, FALSE, TEXT("kbhook-mtx"));
    if (!kbhook_mtx) {
        throw std::runtime_error(GetLastErrorAsString());
    }

    /* Create a block in shared memory. The shmem will hold an int flag
     * that tells us when a key has been pressed. See kbhook lib for details. */
    HANDLE mapped_filed =
        CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0,
                          sizeof(int), "kbhell-shmem");
    if (!mapped_filed) {
        throw std::runtime_error(GetLastErrorAsString());
    }

    /* a counter of the number of key releases lives in shmem */
    int* num_keyreleases = static_cast<int*>(
        MapViewOfFile(mapped_filed, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(int)));
    if (!num_keyreleases) {
        throw std::runtime_error(GetLastErrorAsString());
    }
    *num_keyreleases = 0;

    /* Setup the global keyboard hook. */
    HINSTANCE kbhook_dll = LoadLibrary(TEXT("kbhook.dll"));
    if (!kbhook_dll) {
        throw std::runtime_error(GetLastErrorAsString());
    }
    HOOKPROC kbhook_hook =
        (HOOKPROC)GetProcAddress(kbhook_dll, "KeyReleaseEventCntr");
    if (!kbhook_hook) {
        throw std::runtime_error(GetLastErrorAsString());
    }
    HHOOK kbhook_handle =
        SetWindowsHookEx(WH_GETMESSAGE, kbhook_hook, kbhook_dll, 0);
    if (!kbhook_handle) {
        throw std::runtime_error(GetLastErrorAsString());
    }

    bool exit = false;
    int local_keypress = 0; /* detects a keypress in the app window */
    const char kEsc = static_cast<char>(27); /* ASCII ESC character */
    while (!exit) {
        local_keypress = _kbhit();
        if (local_keypress && (_getch() == kEsc)) {
            exit = true;
        }

        DWORD rc = WaitForSingleObject(kbhook_mtx, 100);
        switch (rc) {
            case WAIT_OBJECT_0:
                if (*num_keyreleases) {
                    player.Play();
                }
                *num_keyreleases = 0;
                ReleaseMutex(kbhook_mtx);
                break;
            case WAIT_ABANDONED:
                exit = true;
                break;
        }
    }

    if (kbhook_mtx) {
        CloseHandle(kbhook_mtx);
    }
    if (mapped_filed) {
        CloseHandle(mapped_filed);
    }
    if (num_keyreleases) {
        UnmapViewOfFile(num_keyreleases);
    }
    if (kbhook_handle) {
        UnhookWindowsHookEx(kbhook_handle);
    }
    if (kbhook_dll) {
        FreeLibrary(kbhook_dll);
    }
}

#endif
