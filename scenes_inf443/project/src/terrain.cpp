#include "terrain.hpp"
#include "utils.hpp"
#include <chrono>

std::vector<utils::Triplet> get_neighbours (utils::Triplet t){
    std::vector<utils::Triplet> neighbour_chunks = {
        t,
        t + utils::Triplet( 1, 0, 0),
        t + utils::Triplet( 1, 1, 0), 
        t + utils::Triplet( 0, 1, 0), 
        t + utils::Triplet(-1, 1, 0), 
        t + utils::Triplet(-1, 0, 0), 
        t + utils::Triplet(-1,-1, 0), 
        t + utils::Triplet( 0,-1, 0), 
        t + utils::Triplet( 1,-1, 0)
    };
    return neighbour_chunks;
}

terrain::terrain(){
    // creating initial chunks
    for (const utils::Triplet& t : get_neighbours({0, 0, 0}))
        chunks[t] = chunk({t.x, t.y});
}

void terrain::draw(const environment_structure& env, bool wireframe, const vec3& player_position, const vec3& player_looking_at, const float& max_depth){
    // get visible chunks
    auto player_chunk = chunk::get_chunk_triplet(player_position);

    std::vector<utils::Triplet> neighbour_chunks = get_neighbours(player_chunk);

    for (const utils::Triplet& t : neighbour_chunks){
        if(!check_chunk_exists(t)) {
            chunks[t] = chunk({t.x, t.y});
        };
        chunks[t].draw(
            env,
            wireframe,
            player_position,
            player_looking_at,
            max_depth
        );
    }
}

std::vector<cube> terrain::get_cubes(const vec3& player_position) {
    auto player_chunk = chunk::get_chunk_triplet(player_position);
    std::vector<cube> cubes;
    for(const auto& chunk : get_neighbours(player_chunk)) {
        auto& ccubes = chunks[chunk].get_cubes();
        cubes.insert(cubes.end(), ccubes.begin(), ccubes.end());
    }
    return cubes;
}


bool terrain::check_chunk_exists(const utils::Triplet t){
    return !(chunks.find(t) == chunks.end());
}

void terrain::create_bloc(const vec3& position, const block_types& block_type){
    vec3 round_position = utils::round(position);
    auto player_chunk = chunk::get_chunk_triplet(round_position);
    chunks[player_chunk].create_block(block_type, round_position);
}

void terrain::delete_bloc(const vec3& position){
    vec3 round_position = utils::round(position);
    auto player_chunk = chunk::get_chunk_triplet(round_position);
    chunks[player_chunk].delete_bloc(round_position);
}