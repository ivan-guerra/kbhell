#ifndef EVENT_LOOP_HPP_
#define EVENT_LOOP_HPP_

#include "wav_player/wav_player.hpp"

namespace kbhell {

/**
 * @brief Play WAV audio everytime a key is released anywhere on the desktop.
 *
 * @param player A WAV audio player object.
 *
 * @throws std::runtime_error When the keypress handler encounters an error
 *                            during setup or execution.
 */
void RunEventLoop(WavPlayer& player);

}  // namespace kbhell

#endif
