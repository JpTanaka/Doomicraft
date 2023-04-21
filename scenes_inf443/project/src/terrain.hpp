#pragma once
#include "cgp/cgp.hpp"
#include "environment.hpp"
#include "cube.hpp"
#include "block.hpp"
#include "utils.hpp"
#include "chunk.hpp"
#include <unordered_map>
#include <functional>

class terrain {

    std::unordered_map<utils::Triplet, chunk, utils::TripletHash> chunks;
    void create_chunk();
    bool check_chunk_exists(const utils::Triplet) const;

public:
    terrain();
    void draw(const environment_structure&, bool wireframe, const vec3& player_position, const vec3& player_looking_at, const float& max_depth);
    std::vector<cube> get_cubes(const vec3& player_position);

    void create_bloc(const vec3& position, const block_types& block_type);
    void delete_bloc(const vec3& position);

    void print_created_chunks(); //debugging 
};