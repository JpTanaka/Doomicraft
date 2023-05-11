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
    std::vector<billboard>transparents;


    for (const utils::Triplet& t : get_neighbours(player_chunk)){
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
        auto& bills = chunks[t].get_billboards();
        transparents.insert(transparents.end(), bills.begin(), bills.end());
    }

	// display transparents
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(false);

    std::sort(transparents.begin(), transparents.end(), 
        [&](const auto& b1, const auto& b2){
            return norm(b1.get_position() - player_position) > norm(b2.get_position() - player_position);
        }
    );

    for (const auto& trans : transparents)
        if (norm(player_position - trans.get_position()) < max_depth)
	        trans.draw(env, wireframe);

	glDepthMask(true);
	glDisable(GL_BLEND);
}

std::vector<cube> terrain::get_cubes(const vec3& player_position) {
    auto player_chunk = chunk::get_chunk_triplet(player_position);
    std::vector<cube> cubes;
    for(const auto& chunk : get_neighbours(player_chunk)) {
        if(!check_chunk_exists(chunk)) continue;
        auto& ccubes = chunks[chunk].get_cubes();
        cubes.insert(cubes.end(), ccubes.begin(), ccubes.end());
    }
    return cubes;
}


bool terrain::check_chunk_exists(const utils::Triplet t) const {
    return !(chunks.find(t) == chunks.end());
}

void terrain::create_bloc(const vec3& position, const block_types& block_type){
    vec3 round_position = utils::round(position);
    auto player_chunk = chunk::get_chunk_triplet(position);
    chunks[player_chunk].create_block_absolute(block_type, round_position);
}

void terrain::delete_bloc(const vec3& position){
    vec3 round_position = utils::round(position);
    auto player_chunk = chunk::get_chunk_triplet(position);
    for (const auto& t : get_neighbours(player_chunk))
        chunks[t].delete_bloc_absolute(round_position);
}

void terrain::print_created_chunks(){
    std::cout << "start " << std::endl;
    for (auto& [key, val] : chunks){
        std::cout<< key << " -- ";
    }
    std::cout << std::endl << " end" << std::endl;
}