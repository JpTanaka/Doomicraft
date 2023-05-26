#pragma once

#include "constants.hpp"
#include "cube.hpp"

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
    std::array<vec3, 6> normals;

    float cos_frac_fov = std::cos(FIELD_OF_VIEW / 3.0f);

public:
    bool transparent = false;
    block_mesh(std::string texture_path_top, std::string texture_path_sides, vec3 color = {0,0,0});
    block_mesh(std::string texture_path);
    block_mesh(const std::tuple<std::string, vec3, bool>*);
    block_mesh();
    void draw(const environment_structure& env, vec3 position, std::vector<directions> render_directions, const vec3& from, bool wireframe);
    void init_meshes();
};

