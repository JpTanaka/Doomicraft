#include "block.hpp"
#include <optional>


std::array<block_mesh, block_types::NUMBER_BLOCKS> block::blocks;

void block::initialize(){
    std::tuple<std::string, vec3, bool> earth[] = {
        {project::path + "assets/grass_top.png",    vec3{0,1,0},    false},
        {project::path + "assets/dirt.png",         vec3{1,1,1},    true},
        {project::path + "assets/grass_side.png",   vec3{1,1,1},    true},
        {project::path + "assets/grass_side.png",   vec3{1,1,1},    true},
        {project::path + "assets/grass_side.png",   vec3{1,1,1},    true},
        {project::path + "assets/grass_side.png",   vec3{1,1,1},    true}
    };
    blocks[block_types::earth] = block_mesh(earth);

    std::tuple<std::string, vec3, bool> wood[] = {
        {project::path + "assets/log_big_oak_top.png",    vec3{1,1,1},    true},
        {project::path + "assets/log_big_oak_top.png",    vec3{1,1,1},    true},
        {project::path + "assets/log_big_oak.png",        vec3{1,1,1},    true},
        {project::path + "assets/log_big_oak.png",        vec3{1,1,1},    true},
        {project::path + "assets/log_big_oak.png",        vec3{1,1,1},    true},
        {project::path + "assets/log_big_oak.png",        vec3{1,1,1},    true}
    };
    blocks[block_types::wood] = block_mesh(wood);

    std::tuple<std::string, vec3, bool> leaf[] = {
        {project::path + "assets/leaves_big_oak_opaque.png",    vec3{0,1,0},    true},
        {project::path + "assets/leaves_big_oak_opaque.png",    vec3{0,1,0},    true},
        {project::path + "assets/leaves_big_oak_opaque.png",    vec3{0,1,0},    true},
        {project::path + "assets/leaves_big_oak_opaque.png",    vec3{0,1,0},    true},
        {project::path + "assets/leaves_big_oak_opaque.png",    vec3{0,1,0},    true},
        {project::path + "assets/leaves_big_oak_opaque.png",    vec3{0,1,0},    true}
    };
    blocks[block_types::leaf] = block_mesh(leaf);

    blocks[block_types::rock] = block_mesh(
        project::path + "assets/stone.png"
    );

    blocks[block_types::brick] = block_mesh(
        project::path + "assets/brick.png"
    );
    blocks[block_types::sand] = block_mesh(
        project::path + "assets/sand.png"
    );
    blocks[block_types::wood_plank] = block_mesh(
        project::path + "assets/planks_big_oak.png"
    );
    blocks[block_types::stone_brick] = block_mesh(
        project::path + "assets/stonebrick.png"
    );
}

block::block(){}
block::block(block_types b_type, vec3 pos)
    :block_cube(pos)
{
    block_type = b_type;
    position = pos;
}

void block::draw(const environment_structure& env, bool wireframe, const vec3& looking_at){
    block_mesh& mesh = blocks[block_type];
    mesh.draw(env, position, render_directions, looking_at, wireframe);
}

bool block::is_being_seen(const vec3& from, const vec3& looking_at, const float& max_depth){
    vec3 block_to_player = position - from;
    float n = 1.0f;
    return (
        (n = norm(block_to_player)) <= max_depth
        && dot(looking_at, (block_to_player)/n) >= cos_fov
    );
}


block_types block::get_next_block(block_types b, int amount){
    if (amount == 1) {
        if (b == NUMBER_BLOCKS - 1) {
            return NO_BLOCK;
        } else if (b == NO_BLOCK) {
            return static_cast<block_types>(0);
        } else {
            return static_cast<block_types>(b + 1);
        }
    } else if (amount == -1) {
        if (b == static_cast<block_types>(0)) {
            return NO_BLOCK;
        } else if (b == NO_BLOCK) {
            return static_cast<block_types>(NUMBER_BLOCKS - 1);
        } else {
            return static_cast<block_types>(b - 1);
        }
    }
    return b;
}