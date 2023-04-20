#pragma once
#include "cgp/cgp.hpp"
#include "environment.hpp"
#include "cube.hpp"
#include "block.hpp"
#include "utils.hpp"
#include "chunk.hpp"
#include <unordered_map>
#include <functional>

class terrain {

    std::unordered_map<utils::Triplet, chunk, utils::TripletHash> chunks;


    // static const int
    //     chunk_x_size = 16,
    //     chunk_y_size = 16,
    //     chunk_z_size = 10;


    // static const int 
    //     xmin = -32,
    //     xmax =  32, 
    //     ymin = -32, 
    //     ymax =  32,
    //     zmin = -2, 
    //     zmax =  10;

    void create_chunk();
    bool check_chunk_exists(const utils::Triplet);

public:
    terrain();
    void draw(const environment_structure&, bool wireframe, const vec3& player_position, const vec3& player_looking_at, const float& max_depth);
    const std::vector<cube> get_cubes(const vec3& player_position);
};