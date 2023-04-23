#pragma once
#include <thread>
#include <iostream>

struct listeners {
    bool shoot = false;
    bool hit = false;
    bool kill = false;
};

extern listeners lists;

void init_sound_effects();

void init_sfx(const char* wav_path, int duration_ms, bool* listener);
