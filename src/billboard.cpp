#include "billboard.hpp"
#include "constants.hpp"
#include "cgp/cgp.hpp"

std::array<std::pair<mesh_drawable, mesh_drawable>, billboard_types::NUM_BILLBOARDS> billboards;

void billboard::init_billboards(){
    for (int i = 0; i < billboard_types::NUM_BILLBOARDS; i++){
    	billboards[i].first.initialize_data_on_gpu(
    		mesh_primitive_quadrangle(
    			Length * vec3{-0.5f, -0.5f, -0.5f}, Length * vec3{0.5f, 0.5f, -0.5f}, Length * vec3{0.5f, 0.5f, 0.5f}, Length * vec3{-0.5f,-0.5f, 0.5f}
    		)
    	);
    	billboards[i].second.initialize_data_on_gpu(
    		mesh_primitive_quadrangle(
    			Length * vec3{0.5f, -0.5f, -0.5f}, Length * vec3{-0.5f, 0.5f, -0.5f}, Length * vec3{-0.5f, 0.5f, 0.5f}, Length * vec3{0.5f,-0.5f, 0.5f}
    		)
    	);
    	billboards[i].first.material.phong = {0.4f, 0.6f, 0, 1};
    	billboards[i].second.material.phong = {0.4f, 0.6f, 0, 1};
    }

    // init textures
    billboards[red_flower].first.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/red_flower.png");
    billboards[red_flower].second.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/red_flower.png");

    billboards[red_tulip].first.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/red_tulip.png");
    billboards[red_tulip].second.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/red_tulip.png");

    billboards[paeonia].first.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/paeonia.png");
    billboards[paeonia].second.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/paeonia.png");

    billboards[blue_flower].first.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/blue_flower.png");
    billboards[blue_flower].second.texture.load_and_initialize_texture_2d_on_gpu(project::path + "assets/blue_flower.png");
}
billboard::billboard(billboard_types bt, vec3 pos)
{
    position = pos;
    this->type = bt;
}

void billboard::draw(const environment_structure& env, bool wireframe) const {
    billboards[type].first.model.translation = position;
    billboards[type].second.model.translation = position;
    if (wireframe) {
        cgp::draw_wireframe(billboards[type].first, env);
        cgp::draw_wireframe(billboards[type].second, env);
    }
    else {
        cgp::draw(billboards[type].first, env);
        cgp::draw(billboards[type].second, env);
    }
}


vec3 billboard::get_position() const{
    return position;
}

void billboard::update_position(vec3 p){
    position = p;
}