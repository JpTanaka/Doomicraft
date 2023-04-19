#pragma once
#include "cube.hpp"
#include "constants.hpp"


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
    block_mesh(std::string texture_path);
    block_mesh();
    void draw(environment_structure& env, vec3 position, std::vector<directions> render_directions, bool wireframe);
};


class block {
    block_types block_type;

    vec3 position;


public:
    static std::array<block_mesh, 2> blocks;

    cube block_cube;

    void draw(environment_structure&, std::vector<directions> render_directions, bool wireframe);

    block();

    block(block_types, vec3);

    static void initialize();

};