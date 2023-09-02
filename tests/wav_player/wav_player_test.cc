#include "wav_player/wav_player.hpp"

#include <gtest/gtest.h>

#include <stdexcept>

TEST(WavPlayerTest, WavPlayerConstructorSucceedsOnValidSoundFile) {
    kbhell::WavPlayer player("data/dot.wav");
}

TEST(WavPlayerTest, WavPlayerConstructorThrowsRuntimeErrorOnInvalidSoundFile) {
    ASSERT_THROW(kbhell::WavPlayer player("invalid.wav"), std::runtime_error);
}
