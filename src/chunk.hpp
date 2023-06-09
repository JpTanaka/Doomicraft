#pragma once
#include "cgp/cgp.hpp"
#include "environment.hpp"
#include "cube.hpp"
#include "block.hpp"
#include "billboard.hpp"
#include "utils.hpp"
#include <unordered_map>
#include <functional>
#include <memory>

class chunk {
    vec2 position;

    std::unordered_map<utils::Triplet, block, utils::TripletHash> blocks;
    std::vector<cube> cubes;

    std::vector<block*> transparents;
    std::vector<billboard> billboards;

    int generator_function(const vec2&);


    void create_tree(const vec3&);
    void create_flower(const vec3&);

    bool check_has_block(const utils::Triplet& t);

    void create_block(const block_types&, const vec3&);

    void update_blocks();


public:
    chunk();
    chunk(vec2 position);
    void draw(const environment_structure&, bool wireframe, const vec3& player_position, const vec3& player_looking_at, const float& max_depth);
    std::vector<cube>& get_cubes();
    std::vector<billboard>& get_billboards();

    void create_block_absolute(const block_types&, const vec3&);
    void delete_bloc_absolute(vec3 position);

    static utils::Triplet get_chunk_triplet(const vec3& pos);

    static const int
        chunk_x_size = 16,
        chunk_y_size = 16,
        chunk_z_size = 10;
};

