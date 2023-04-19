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

    for (mesh_drawable mesh: meshes)
        mesh.texture.load_and_initialize_texture_2d_on_gpu(texture_path);

}

void block_mesh::draw(environment_structure& env, vec3 position, std::vector<directions> render_directions, bool wireframe){
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

void block::draw(environment_structure& env, std::vector<directions> render_directions, bool wireframe){
    block_mesh& mesh = blocks[block_type];
    mesh.draw(env, position, render_directions, wireframe);
}