#pragma once

#include "cgp/cgp.hpp"
#include "camera.hpp"
#include "cube.hpp"
#include "constants.hpp"

class character {

    float jump_velocity = 5;

public:
    cube body;
    cube legs;

    vec3 position;
    vec2 direction;
    vec3 velocity;

    bool is_jumping;
    character(vec3 center);
    character();

    virtual void move(const std::vector<cube>&) = 0;

    float bottom();
    float top();

    vec3 get_eyes();
};
