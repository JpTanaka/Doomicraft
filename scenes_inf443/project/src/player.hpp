#pragma once

#include "cgp/cgp.hpp"
#include "camera.hpp"
#include "cube.hpp"
#include "constants.hpp"
#include "character.hpp"

class player : character {

    float jump_velocity = 5;

public:
    camera_controller_custom* camera;
    bool* creative;

    player(camera_controller_custom&, vec3 center, bool* creative);
    player();

    void move(const std::vector<cube>&);
};