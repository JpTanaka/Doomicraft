#pragma once
#include "cube.hpp"
#include "constants.hpp"
#include "block_mesh.hpp"
#include <optional>


enum block_types {
    earth,
    rock,
    brick,
    stone_brick,
    sand,
    wood_plank,
    wood,
    leaf,
    NUMBER_BLOCKS,
    NO_BLOCK
};
class block{

    block_types block_type;
    float cos_fov = std::cos(FIELD_OF_VIEW);

public:

    vec3 position;

    std::vector<directions> render_directions;

    static std::array<block_mesh, block_types::NUMBER_BLOCKS> blocks;

    cube block_cube;

    void draw(const environment_structure&, bool wireframe, const vec3& from) const;


    block();

    block(block_types, vec3);

    bool is_being_seen(const vec3& from, const vec3& looking_at, const float& max_depth) const;

    static void initialize();

    static block_types get_next_block(block_types, int amount);

    bool is_transparent() const;

    vec3 get_position() const;
};