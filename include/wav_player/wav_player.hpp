#ifndef WAV_PLAYER_HPP_
#define WAV_PLAYER_HPP_

#include <SDL2/SDL_mixer.h>

#include <string>

namespace kbhell {

class WavPlayer {
   public:
    WavPlayer() = delete;
    explicit WavPlayer(const std::string& sound_file);
    ~WavPlayer();
    WavPlayer(const WavPlayer&) = delete;
    WavPlayer(WavPlayer&&) = delete;
    WavPlayer& operator=(const WavPlayer&) = delete;
    WavPlayer& operator=(WavPlayer&&) = delete;

    void Play();

   private:
    Mix_Chunk* soundbite_;
};

}  // namespace kbhell

#endif
