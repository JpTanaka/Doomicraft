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

bool cube::colision(cube const& c){
    return std::max({
        std::abs(c.position.x - position.x),
        std::abs(c.position.y - position.y),
        std::abs(c.position.z - position.z)
    }) <= L;
}
float cube::distance(cube const& c){
    float dx = std::abs(c.position.x - position.x);
    float dy = std::abs(c.position.y - position.y);
    float dz = std::abs(c.position.z - position.z);

    std::cout << "dx " << dx<< " dy " << dy<< " dz " << dz <<std::endl;

    return std::max({dx, dy, dz});
}