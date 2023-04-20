#pragma once

#include "cgp/cgp.hpp"
#include "camera.hpp"
#include "cube.hpp"
#include "constants.hpp"

class character {


public:
    float jump_velocity = 5;
    cube body;
    cube legs;

    vec3 position;
    vec2 direction;
    vec3 velocity;

    bool is_jumping;
    character(vec3 center);
    character();

    float bottom();
    float top();

    vec3 get_eyes();
};
