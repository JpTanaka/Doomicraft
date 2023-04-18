#include "block.hpp"


void block::initialize(){
    mesh_drawable earth;
    earth.initialize_data_on_gpu(mesh_primitive_cube({0,0,0}, Length));
    earth.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/dirt.jpg");
    // earth.material.color = {0.6, 0.3, 0};
    blocks.push_back(earth);

    mesh_drawable rock;
    rock.initialize_data_on_gpu(mesh_primitive_cube({0,0,0}, Length));
    rock.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/stone.jpg");
    // rock.material.color = {0.5, 0.5, 0.5};
    blocks.push_back(rock);
}

block::block(){}
block::block(block_types b_type, vec3 pos)
    :block_cube(pos)
{
    block_type = b_type;
    position = pos;
}

void block::draw(environment_structure& env, bool wireframe){
    mesh_drawable& mesh = blocks[block_type];
    mesh.model.translation = position;
    if(!wireframe)
	    cgp::draw(mesh, env);
    else
        cgp::draw_wireframe(mesh, env);
}