#ifndef WAV_PLAYER_HPP_
#define WAV_PLAYER_HPP_

#include <SDL_mixer.h>

#include <string>

namespace kbhell {

/**
 * @brief WavPlayer is a utility for playing a WAV file over the PC speakers.
 *
 * WavPlayer uses the Simple DirectMedia Layer library to play WAVE audio over
 * the PC's speakers. A WavPlayer instance is only capable of playing the WAV
 * file that was passed at construction. Multiple WavPlayer instances are
 * needed if one wishes to play multiple sound files.
 */
class WavPlayer {
   public:
    /**
     * @brief Construct a WavPlayer object that can play the param WAV file.
     *
     * @param sound_file Path to a WAV sound file.
     *
     * @throws std::runtime_error When unable to initialize the audio system or
     *                            when \p sound_file points to a nonexistent
     *                            WAV file.
     */
    explicit WavPlayer(const std::string& sound_file);

    /**
     * @brief WavPlayer destructor.
     *
     * The WavPlayer destructor takes care of releasing audio resources on
     * behalf of the caller.
     */
    ~WavPlayer();

    WavPlayer() = delete;
    WavPlayer(const WavPlayer&) = delete;
    WavPlayer(WavPlayer&&) = delete;
    WavPlayer& operator=(const WavPlayer&) = delete;
    WavPlayer& operator=(WavPlayer&&) = delete;

    /**
     * @brief Play the WAV audio file passed in at construction.
     *
     * Consecutive calls to Play() in which the audio would overlap causes the
     * audio to play from the beginning.
     *
     * @throws std::runtime_error When the audio cannot be played.
     */
    void Play();

   private:
    Mix_Chunk* soundbite_;
};

}  // namespace kbhell

#endif
