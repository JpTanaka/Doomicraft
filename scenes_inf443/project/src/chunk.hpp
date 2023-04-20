#pragma once
#include "cgp/cgp.hpp"
#include "environment.hpp"
#include "cube.hpp"
#include "block.hpp"
#include "utils.hpp"
#include <unordered_map>
#include <functional>

class chunk {
    vec2 position;

    std::unordered_map<utils::Triplet, block, utils::TripletHash> blocks;
    std::vector<cube> cubes;

    int generator_function(const vec2&);

    void create_block(const block_types&, const vec3&);

    void create_tree(const vec3&);

    bool check_has_block(const utils::Triplet& t);


public:
    chunk();
    chunk(vec2 position);
    void draw(const environment_structure&, bool wireframe, const vec3& player_position, const vec3& player_looking_at, const float& max_depth);
    const std::vector<cube> get_cubes();

    static const int
        chunk_x_size = 16,
        chunk_y_size = 16,
        chunk_z_size = 10;
};

