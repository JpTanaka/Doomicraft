#pragma once
#include "cube.hpp"
#include "constants.hpp"
#include "block_mesh.hpp"
#include <optional>


enum block_types {
    earth,
    rock,
    wood_plank,
    brick,
    sand,
    stone_brick,
    wood,
    leaf,
    NUMBER_BLOCKS,
    NO_BLOCK
};
class block {

    block_types block_type;

public:

    vec3 position;

    std::vector<directions> render_directions;

    static std::array<block_mesh, block_types::NUMBER_BLOCKS> blocks;

    cube block_cube;

    void draw(const environment_structure&, bool wireframe);

    block();

    block(block_types, vec3);

    bool is_being_seen(const vec3& from, const vec3& looking_at, const float& max_depth);

    static void initialize();

    static block_types get_next_block(block_types, int amount);
};