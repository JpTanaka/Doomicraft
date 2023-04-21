#pragma once
#include "cgp/cgp.hpp"
#include "environment.hpp"
#include "cube.hpp"
#include "terrain.hpp"
#include "utils.hpp"
#include <unordered_map>
#include <functional>
#include "mob.hpp"
#include "constants.hpp"

class mob_group {

    std::vector<mob> mobs;

    static const int mob_range = 16;
    static const int wave_size = 5;

public:

    mob_group();
    mob_group(vec3 starting_position);

    std::vector<cube> get_cubes();

    void draw(const environment_structure&, bool wireframe);

    void move(terrain&, vec3&, const float &);

    void add_mob(const mob);

    void shoot_mob(
        vec3, // point from where the ray is being shot
        vec3, // direction of the ray
        float // distance;
    );

    int check_dead();

    bool check_hits_player(vec3 player_position);
        
};
