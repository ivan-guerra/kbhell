/**
 * @file kbhook.hpp
 * @brief Implement a Windows global hook signaling a key release event.
 *
 * The kbhook module implements a Windows global hook that signals to the main
 * application driver when a key release event has occurred. Windows requires
 * that global hooks be implemented in a seperate DLL that is why you find this
 * hook seperate from the main driver. Internally, KeyReleaseEventCntr()
 * increments a counter in shared memory whenever a key release event occurs.
 * The application driver polls this counter in order to determine whether
 * a sound should be played. The application driver owns the shared memory and
 * a mutex used to synchronize access to the counter. kbhook gets references
 * to shared memory and the named mutex during DLL initialization and releases
 * these resources on DLL detachment.
 */
#ifndef KBHOOK_HPP_
#define KBHOOK_HPP_

#include <windows.h>

/*!
 * @brief A global hook associated with the WH_GETMESSAGE hook ID.
 *
 * See Microsoft's <a
 * href="https://learn.microsoft.com/en-us/windows/win32/winmsg/getmsgproc">GetMsgProc()</a>
 * docs for details.
 */
extern "C" __declspec(dllexport) LRESULT CALLBACK
    KeyReleaseEventCntr(int nCode, WPARAM wParam, LPARAM lParam);

#endif
