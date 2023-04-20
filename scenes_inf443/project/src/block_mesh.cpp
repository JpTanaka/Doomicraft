
#include "block_mesh.hpp"

block_mesh::block_mesh(){}

block_mesh::block_mesh(std::string texture_path){
    init_meshes();
    for (int i = 0; i < 6; i++){
        mesh_drawable& mesh = meshes[i];
        mesh.material.phong.specular = 0;
        mesh.texture.load_and_initialize_texture_2d_on_gpu(texture_path);
    }
}
block_mesh::block_mesh(std::string texture_path_top, std::string texture_path_sides, vec3 color){
    init_meshes();
    meshes[directions::kTop].texture.load_and_initialize_texture_2d_on_gpu(texture_path_top);
    for (int i = 0; i < 6; i++){
        mesh_drawable& mesh = meshes[i];
        mesh.material.phong.specular = 0;
        if (i == directions::kTop) continue;
        mesh.texture.load_and_initialize_texture_2d_on_gpu(texture_path_sides);
    }
}

block_mesh::block_mesh(const std::tuple<std::string, vec3, bool>* config){
    init_meshes();
    for (int i = 0; i < 6; i++){
        auto& texture_path = get<0>(config[i]);
        auto& color = get<1>(config[i]);
        auto& transparent = get<2>(config[i]);
        mesh_drawable& mesh = meshes[i];
        mesh.texture.load_and_initialize_texture_2d_on_gpu(texture_path);
        mesh.material.color = color;
        mesh.material.phong.specular = 0;
        mesh.material.alpha = transparent ? 1 : 0;
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

void block_mesh::init_meshes(){
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
            { l,-l,-l},
            { l, l,-l},
            { l, l, l},
            { l,-l, l}
        )
    );
    meshes[directions::kBack].initialize_data_on_gpu(
        mesh_primitive_quadrangle(
            {-l, l,-l},
            {-l,-l,-l},
            {-l,-l, l},
            {-l, l, l}
        )
    );
    meshes[directions::kLeft].initialize_data_on_gpu(
        mesh_primitive_quadrangle(
            {-l, l,-l},
            { l, l,-l},
            { l, l, l},
            {-l, l, l}
        )
    );
    meshes[directions::kRight].initialize_data_on_gpu(
        mesh_primitive_quadrangle(
            { l,-l,-l},
            {-l,-l,-l},
            {-l,-l, l},
            { l,-l, l}
        )
    );
}
