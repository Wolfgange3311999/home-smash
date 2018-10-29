#pragma once

#include <map>
#include "soundbank.h"  // Soundbank definitions
#include "audio.hpp"
#include <maxmod9.h>

enum class Sound : int {
    None = -1,
    CowHurt = SFX_COW_HURT,
    PigHurt = SFX_PIG_HURT,
    PlayerHurt = SFX_PLAYER_HURT,
    SheepHurt = SFX_SHEEP_HURT,
    ZombieHurt = SFX_ZOMBIE_HURT,
    Pop = SFX_POP,
    Click = SFX_CLICK,
    DoorClose = SFX_DOOR_CLOSE,
    DoorOpen = SFX_DOOR_OPEN,
    Explode = SFX_EXPLODE
};

enum class SoundType {
    Step = 0,
    Destroy = 1,
    Place = 1
};

enum class SoundAudio {
    Wood,
    Stone,
    Snow,
    Sand,
    Ladder,
    Gravel,
    Grass,
    Cloth
};

enum class Music {
    Stream = -2,
    None = -1,
    Calm = MOD_CALM,
    Hal2 = MOD_HAL2
};

class SoundSystem {
    static SoundSystem *instance;
public:
    SoundSystem();
    void stopStream();
    void update();
    s16 volumeFunc(s16 orig, u16 factor);
    mm_word stream(mm_word length, mm_addr dest, mm_stream_formats format);
    void playStreamSong();
    int getBlockPanning(int x, int camX);
    void playSound(Sound sfx, mm_byte volume, mm_byte panning);
    void playMusic(Music song);
    void stopMusic();


private:
    static mm_word streamFromInstance(mm_word length, mm_addr dest, mm_stream_formats format);
    void loadSound(Sound sfx);

    std::map<std::pair<SoundAudio, SoundType>, std::pair<int, int>> sfxs;
    int soundsPos = 0;
    Sound loadedSounds[8] = {Sound::None}; // circular buffer (must be power of two)
    Music loadedMusic = Music::None;
    bool streamOpen = false;
    bool reqStreamClose = false;
    FILE *file;
    WaveInfo w;
};
