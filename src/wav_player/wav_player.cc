#include "wav_player/wav_player.hpp"

#include <SDL.h>
#include <SDL_mixer.h>

#include <stdexcept>
#include <string>

namespace kbhell {

WavPlayer::WavPlayer(const std::string& sound_file) : soundbite_(nullptr) {
    /* SDL_Init() is optional since we later call Mix_OpenAudio(), the SDL2 docs
     * recommend init'ing anyways so we do it */
    if (SDL_Init(SDL_INIT_AUDIO)) {
        throw std::runtime_error(SDL_GetError());
    }

    /* using the default values recommended in the SDL2 docs:
     * https://wiki.libsdl.org/SDL2_mixer/Mix_OpenAudio */
    const int kDefaultFrequencyHz = 48000;
    const Uint16 kDefaultFormat = AUDIO_S16SYS;
    const int kDefaultNumChannels = 2; /* 2 = stereo */
    const int kDefaultChunkSize = 2048;
    if (Mix_OpenAudio(kDefaultFrequencyHz, kDefaultFormat, kDefaultNumChannels,
                      kDefaultChunkSize)) {
        throw std::runtime_error(Mix_GetError());
    }

    /* load the WAV sounds for dot and dash into memory */
    soundbite_ = Mix_LoadWAV(sound_file.c_str());
    if (!soundbite_) {
        throw std::runtime_error(Mix_GetError());
    }
}

WavPlayer::~WavPlayer() {
    if (soundbite_) {
        Mix_FreeChunk(soundbite_);
    }
    Mix_CloseAudio();
    SDL_Quit();
}

void WavPlayer::Play() {
    const int kDefaultChannel = 1;
    const int kPlayOnce = 0;

    int channel = Mix_PlayChannel(kDefaultChannel, soundbite_, kPlayOnce);
    if (-1 == channel) {
        throw std::runtime_error(Mix_GetError());
    }
}

}  // namespace kbhell
