#pragma once

#include "constants.hpp"
#include "cube.hpp"

enum block_types {
    earth,
    rock
};

enum directions {
    kTop,
    kBottom,
    kLeft,
    kRight,
    kFront,
    kBack
};

class block_mesh {
    std::array<mesh_drawable, 6> meshes;

public:
    block_mesh(std::string texture_path_top, std::string texture_path_sides, vec3 color = {0,0,0});
    block_mesh(std::string texture_path);
    block_mesh(const std::tuple<std::string, vec3, bool>*);
    block_mesh();
    void draw(const environment_structure& env, vec3 position, std::vector<directions> render_directions, bool wireframe);
    void init_meshes();
};

