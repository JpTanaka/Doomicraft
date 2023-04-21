#pragma once

#include "cgp/cgp.hpp"
#include "camera.hpp"
#include "cube.hpp"
#include "constants.hpp"
#include "character.hpp"
#include "block.hpp"
#include "utils.hpp"

class mob : public character {
    static const int max_life = 4;
    int life = max_life;
    mesh_drawable mesh;
    mesh_drawable health_bar;

public:
    mob(vec3 center);
    mob();
    vec3 move_direction_xy;

    void move(const std::vector<cube>&, vec3&, const float &);
    void draw(const environment_structure& env, bool);

    void take_damage();
    bool is_dead();

};