#pragma once
#include "cube.hpp"
#include "constants.hpp"

enum block_types {
    earth,
    rock
};

static std::vector<mesh_drawable> blocks;

class block {
    block_types block_type;

    vec3 position;

public:
    cube block_cube;

    void draw(environment_structure&, vec3 eyes);

    block();

    block(block_types, vec3);

    static void initialize();

};