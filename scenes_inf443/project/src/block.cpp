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
}

block::block(){}
block::block(block_types b_type, vec3 pos)
    :block_cube(pos)
{
    block_type = b_type;
    position = pos;
}

void block::draw(const environment_structure& env, bool wireframe){
    block_mesh& mesh = blocks[block_type];
    mesh.draw(env, position, render_directions, wireframe);
}

bool block::is_being_seen(const vec3& from, const vec3& looking_at, const float& max_depth){
    vec3 block_to_player = position - from;
    return (
        dot(normalize(looking_at), normalize(block_to_player)) >= std::cos(FIELD_OF_VIEW)
        && norm(block_to_player) < max_depth
    );
}


block_types block::get_next_block(block_types b, int amount){
    if (amount == 1) {
        if (b == leaf) {
            return NO_BLOCK;
        } else if (b == NO_BLOCK) {
            return earth;
        } else {
            return static_cast<block_types>(b + 1);
        }
    } else if (amount == -1) {
        if (b == earth) {
            return NO_BLOCK;
        } else if (b == NO_BLOCK) {
            return leaf;
        } else {
            return static_cast<block_types>(b - 1);
        }
    }
    return b;
}