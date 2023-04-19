#pragma once
#include "cgp/cgp.hpp"
#include "environment.hpp"
#include "cube.hpp"
#include "block.hpp"
#include "utils.hpp"
#include <unordered_map>
#include <functional>

class terrain {

    int generator_function(int x, int y);
    std::unordered_map<utils::Triplet, block, utils::TripletHash> blocks;
    std::vector<cube> cubes;

    std::vector<std::tuple<double, vec2, vec2>> gaussians;


    static const int 
        xmin = -32,
        xmax =  32, 
        ymin = -32, 
        ymax =  32,
        zmin = -2, 
        zmax =  10;

public:
    terrain();
    ~terrain();
    void draw(const environment_structure&, bool wireframe, const vec3& player_position, const vec3& player_looking_at, const float& max_depth);
    const std::vector<cube> get_cubes();
};