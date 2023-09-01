#include "kbhook/kbhook.hpp"

#include <windows.h>

static HANDLE kbhook_mtx;
static HANDLE mapped_file;
static int* num_keyreleases;

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, PVOID pvReserved) {
    kbhook_mtx = nullptr;
    num_keyreleases = nullptr;
    mapped_file = nullptr;
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            kbhook_mtx = OpenMutex(MUTEX_ALL_ACCESS, FALSE, TEXT("kbhook-mtx"));
            if (!kbhook_mtx) {
                return FALSE;
            }

            mapped_file =
                OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "kbhell-shmem");
            if (!mapped_file) {
                CloseHandle(kbhook_mtx);
                return FALSE;
            }

            num_keyreleases = static_cast<int*>(MapViewOfFile(
                mapped_file, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(int)));
            if (!num_keyreleases) {
                CloseHandle(kbhook_mtx);
                CloseHandle(mapped_file);
                return FALSE;
            }
            break;
        case DLL_PROCESS_DETACH:
            if (kbhook_mtx) {
                CloseHandle(kbhook_mtx);
            }
            if (num_keyreleases) {
                UnmapViewOfFile(num_keyreleases);
            }
            if (mapped_file) {
                CloseHandle(mapped_file);
            }
            break;
    }
    return TRUE;
}

LRESULT CALLBACK KeyReleaseEventCntr(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode < 0) { /* do not proces message */
        return CallNextHookEx(nullptr, nCode, wParam, lParam);
    }

    const int kMutexDelayMs = 100;
    LPMSG pmsg = (LPMSG)lParam;
    if (WM_KEYUP == pmsg->message) {
        DWORD rc = WaitForSingleObject(kbhook_mtx, kMutexDelayMs);
        switch (rc) {
            case WAIT_OBJECT_0:
                *num_keyreleases += 1;
                ReleaseMutex(kbhook_mtx);
                break;
            case WAIT_ABANDONED:
                /* TODO: The mutex got abandoned, report some error state? */
                break;
        }
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}
