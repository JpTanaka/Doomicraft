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
    mesh_drawable mesh_head;
    mesh_drawable mesh_body;
    mesh_drawable health_bar;

    void rotate_mesh(vec3 to);

public:
    mob(vec3 center);
    mob();
    vec3 move_direction_xy;

    void move(const std::vector<cube>&, vec3&, const float &, const float& move_sensibility);
    void draw(const environment_structure& env, bool);

    void take_damage();
    bool is_dead();
};