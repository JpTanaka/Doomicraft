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
    void draw(const environment_structure& env, vec3 position, std::vector<directions> render_directions, bool wireframe);
    void draw_all(const environment_structure& env, vec3 position, bool wireframe);
};


class block {

    block_types block_type;

    vec3 position;


public:
    std::vector<directions> render_directions;

    static std::array<block_mesh, NUM_BLOCKS> blocks;

    cube block_cube;

    void draw(const environment_structure&, bool wireframe);

    block();

    block(block_types, vec3);

    static void initialize();

};