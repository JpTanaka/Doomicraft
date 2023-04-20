#pragma once
#include "cube.hpp"
#include "constants.hpp"
#include "block_mesh.hpp"

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

    bool is_being_seen(const vec3& from, const vec3& looking_at, const float& max_depth);

    static void initialize();

};