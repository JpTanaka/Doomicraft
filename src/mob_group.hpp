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

    int mob_range = 16;
    int wave_size = 5;
    float velocity = 2;

    int frame_counter = 0;
    int spawn_delay = 60 * 20;

public:

    mob_group();
    mob_group(vec3 starting_position);

    std::vector<cube> get_cubes();

    void draw(const environment_structure&, bool wireframe);

    void move(terrain&, vec3&, const float &);

    void add_mob(const mob);

    bool shoot_mob(
        vec3, // point from where the ray is being shot
        vec3, // direction of the ray
        float // distance;
    );

    int check_dead(vec3 from);

    bool check_hits_player(vec3 player_position);

    void set_level(int);

    void update_mobs(const vec3& from);
        
};
