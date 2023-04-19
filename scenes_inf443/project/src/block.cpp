#include "block.hpp"

block_mesh::block_mesh(){}

block_mesh::block_mesh(std::string texture_path){

    float l = Length/2.0f;
    meshes[directions::kTop].initialize_data_on_gpu(
        mesh_primitive_quadrangle(
            { l, l, l},
            {-l, l, l},
            {-l,-l, l},
            { l,-l, l}
        )
    );
    meshes[directions::kBottom].initialize_data_on_gpu(
        mesh_primitive_quadrangle(
            { l, l,-l},
            { l,-l,-l},
            {-l,-l,-l},
            {-l, l,-l}
        )
    );
    meshes[directions::kFront].initialize_data_on_gpu(
        mesh_primitive_quadrangle(
            { l, l, l},
            { l,-l, l},
            { l,-l,-l},
            { l, l,-l}
        )
    );
    meshes[directions::kBack].initialize_data_on_gpu(
        mesh_primitive_quadrangle(
            {-l, l, l},
            {-l, l,-l},
            {-l,-l,-l},
            {-l,-l, l}
        )
    );
    meshes[directions::kLeft].initialize_data_on_gpu(
        mesh_primitive_quadrangle(
            { l, l, l},
            {-l, l, l},
            {-l, l,-l},
            { l, l,-l}
        )
    );
    meshes[directions::kRight].initialize_data_on_gpu(
        mesh_primitive_quadrangle(
            { l,-l, l},
            { l,-l,-l},
            {-l,-l,-l},
            {-l,-l, l}
        )
    );

    for (mesh_drawable& mesh : meshes){
        mesh.texture.load_and_initialize_texture_2d_on_gpu(texture_path);
        mesh.material.phong.specular = 0;
    }

}

void block_mesh::draw(const environment_structure& env, vec3 position, std::vector<directions> render_directions, bool wireframe){
    for (directions dir : render_directions){
        mesh_drawable& mesh = meshes[dir];
        mesh.model.translation = position;
        if(!wireframe) cgp::draw(mesh, env);
        else cgp::draw_wireframe(mesh, env);
    }
}


std::array<block_mesh, 2> block::blocks;

void block::initialize(){
    blocks[block_types::earth] = block_mesh(
        project::path + "assets/dirt.jpg"
    );
    blocks[block_types::rock] = block_mesh(
        project::path + "assets/stone.jpg"
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