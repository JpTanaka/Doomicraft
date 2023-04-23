#pragma once
#include <thread>
#include <iostream>

struct listeners {
    bool shoot = false;
    bool hit = false;
    bool kill = false;
    bool background = false;
    bool background_creative = false;
    bool death = false;
    bool place_block = false;
};

extern listeners lists;

void init_sound_effects();

void init_sfx(const char* wav_path, int duration_ms, bool* listener, bool on_repeat);
