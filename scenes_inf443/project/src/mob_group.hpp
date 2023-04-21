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
    vec2 position;
    std::vector<cube> cubes;

    public:
    std::vector<mob> mobs;
        mob_group();
        void draw(const environment_structure&, bool wireframe);
        const std::vector<mob>& get_mobs();
        std::vector<cube> get_cubes();
        void move(terrain&, vec3&, const float &);
        void add_mob(const mob&);
        void erase_mob(auto it);
        void shoot_mob(
            vec3, // point from where the ray is being shot
            vec3, // direction of the ray
            float // distance;
        );
        
};
