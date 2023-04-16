#include "cube.hpp"
#include <initializer_list>
#include <algorithm>

cube::cube(vec3 center){
    mesh.initialize_data_on_gpu(mesh_primitive_cube(center, L));
    mesh.material.color = {1, 0, 0};
    position = center;
};
cube::cube(){};

void cube::draw(environment_structure& env){
	cgp::draw(mesh, env);
}


vec3 cube::colision(cube const& c){
    float dx = c.position.x - position.x;
    float dy = c.position.y - position.y;
    float dz = c.position.z - position.z;
    
    float adx = std::abs(dx);
    float ady = std::abs(dy);
    float adz = std::abs(dz);

    auto sign = [](float x) -> float { return x < 0 ? -1 : 1; };


    if( !check_colision(c) ) return {0, 0, 0};

    if (adx >= ady && adx >= adz) return {sign(dx), 0, 0};
    if (ady >= adx && ady >= adz) return {0, sign(dy), 0};
    if (adz >= ady && adz >= adx) return {0, 0, sign(dz)};

    return {0, 0, 0};
}

bool cube::check_colision(cube const& c){
    return distance(c) < L;
}


float cube::distance(cube const& c){
    float dx = std::abs(c.position.x - position.x);
    float dy = std::abs(c.position.y - position.y);
    float dz = std::abs(c.position.z - position.z);
    return std::max({dx, dy, dz});
}