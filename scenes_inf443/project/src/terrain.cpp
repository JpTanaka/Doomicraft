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
    utils::Triplet player_chunk(
        floor(player_position.x / static_cast<float>(chunk::chunk_x_size)), 
        floor(player_position.y / static_cast<float>(chunk::chunk_y_size)),
        0
    );

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

const std::vector<cube> terrain::get_cubes(const vec3& player_position){
    utils::Triplet player_chunk(
        floor(player_position.x / static_cast<float>(chunk::chunk_x_size)), 
        floor(player_position.y / static_cast<float>(chunk::chunk_y_size)),
        0
    );
    return chunks[player_chunk].get_cubes();
}


bool terrain::check_chunk_exists(const utils::Triplet t){
    return !(chunks.find(t) == chunks.end());
}