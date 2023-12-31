#include "kbhell/event_loop.hpp"
#include "wav_player/wav_player.hpp"

#include <cstdio>
#include <iostream>
#include <stdexcept>

#ifdef __linux__
#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include <X11/extensions/record.h>

struct CallbackData {
  ::Display *ctrl_disp = nullptr;
  ::XRecordContext *ctx = nullptr;
  kbhell::WavPlayer *player = nullptr;
};

/* refer to libxnee */
union XRecordDatum {
  unsigned char type;
  xEvent event;
  xResourceReq req;
  xGenericReply reply;
  xError error;
  xConnSetupPrefix setup;
};

void KeyCallback(XPointer closure, XRecordInterceptData *hook) {
  if (hook->category != XRecordFromServer) {
    ::XRecordFreeData(hook);
    return;
  }

  CallbackData *cb_data = reinterpret_cast<CallbackData *>(closure);
  XRecordDatum *data = reinterpret_cast<XRecordDatum *>(hook->data);

  int event_type = data->type;
  BYTE keycode = data->event.u.u.detail;
  const int kEsc = 9;
  switch (event_type) {
  case KeyRelease:
    if (keycode == kEsc) { /* if ESC is pressed at any time, exit */
      ::XRecordDisableContext(cb_data->ctrl_disp, *cb_data->ctx);
    } else {
      cb_data->player->Play();
    }
    break;
  default:
    break;
  }
  ::XRecordFreeData(hook);
}

void kbhell::RunEventLoop(WavPlayer &player) {
  ::Display *ctrl_disp = XOpenDisplay(nullptr);
  ::Display *data_disp = XOpenDisplay(nullptr);
  if (!ctrl_disp || !data_disp) {
    throw std::runtime_error("unable to open X display");
  }

  int major = 0;
  int minor = 0;
  if (!::XRecordQueryVersion(ctrl_disp, &major, &minor)) {
    throw std::runtime_error("RECORD extension not supported on this X server");
  }

  /* we must set the ctrl_disp to sync mode, or, when we enable the context
   * in data_disp, there will be a fatal X error */
  ::XSynchronize(ctrl_disp, true);

  ::XRecordRange *record_rng = ::XRecordAllocRange();
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

  CallbackData cb_data{
      .ctrl_disp = ctrl_disp,
      .ctx = &record_ctx,
      .player = &player,
  };
  if (!::XRecordEnableContext(data_disp, record_ctx, KeyCallback,
                              reinterpret_cast<::XPointer>(&cb_data))) {
    throw std::runtime_error("could not enable record context");
  }

  ::XRecordProcessReplies(data_disp);

  ::XRecordFreeContext(ctrl_disp, record_ctx);
  ::XFree(record_rng);
  ::XCloseDisplay(data_disp);
  ::XCloseDisplay(ctrl_disp);
}

#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <conio.h>
#include <windows.h>

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <string>
#include <thread>

static bool key_released = false;
static std::mutex key_released_mtx;
static std::condition_variable key_released_cv;
static std::atomic_bool exit_event_loop(false);

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

LRESULT CALLBACK KeyCallback(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode < 0) {
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
  }

  KBDLLHOOKSTRUCT *kbinfo = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);
  if (wParam == WM_KEYUP) {
    std::unique_lock<std::mutex> lock(key_released_mtx);
    key_released = true;
    if (VK_ESCAPE == kbinfo->vkCode) {
      exit_event_loop = true; /* signal the main driver thread to exit */
      PostQuitMessage(0);     /* signal this kbd hook thread to exit */
    }
    key_released_cv.notify_one();
  }

  return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

void InstallHook() {
  HHOOK kbd_hook = SetWindowsHookEx(WH_KEYBOARD_LL, &KeyCallback, 0, 0);
  if (!kbd_hook) {
    return;
  }

  MSG message;
  while (GetMessage(&message, nullptr, 0, 0)) {
    DispatchMessage(&message);
  }

  UnhookWindowsHookEx(kbd_hook);
}

void kbhell::RunEventLoop(WavPlayer &player) {
  /* launch a seperate thread hosting a low level keyboard hook */
  std::thread kbd_event_thrd(InstallHook);

  while (!exit_event_loop) {
    std::unique_lock<std::mutex> lock(key_released_mtx);
    /* wait until a key release event has occurred */
    key_released_cv.wait(lock, [] { return key_released; });

    player.Play();
    key_released = false;
  }

  if (kbd_event_thrd.joinable()) {
    kbd_event_thrd.join();
  }
}

#endif
