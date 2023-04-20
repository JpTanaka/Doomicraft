#include "block.hpp"

std::array<block_mesh, 2> block::blocks;

void block::initialize(){
    std::tuple<std::string, vec3, bool> config[]  = {
        {project::path + "assets/grass_top.png",    vec3{0,1,0},    false},
        {project::path + "assets/dirt.png",         vec3{1,1,1},    true},
        {project::path + "assets/grass_side.png",   vec3{1,1,1},    true},
        {project::path + "assets/grass_side.png",   vec3{1,1,1},    true},
        {project::path + "assets/grass_side.png",   vec3{1,1,1},    true},
        {project::path + "assets/grass_side.png",   vec3{1,1,1},    true}
    };
    blocks[block_types::earth] = block_mesh(config);
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