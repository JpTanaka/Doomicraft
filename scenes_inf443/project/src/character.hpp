#pragma once

#include "cgp/cgp.hpp"
#include "camera.hpp"
#include "cube.hpp"
#include "constants.hpp"

class character {

public:
    camera_controller_custom* camera;
    cube body;

    vec3 position;
    vec2 direction;

    float velocity_z;
    bool is_jumping;

    character(camera_controller_custom&, vec3 center);
    character();

    void move(std::vector<cube>);
};
