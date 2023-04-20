#include "terrain.hpp"
#include "utils.hpp"
#include <chrono>

int terrain::generator_function(int x, int y){

    double z = 0;
    for (auto tup : gaussians){
        z += get<0>(tup) * utils::gaussian({x,y}, get<1>(tup), get<2>(tup));
    }
    z += 3.0f * cgp::noise_perlin(0.05*vec2{x, y}, 1);
    z += 0.5f * cgp::noise_perlin(0.05*vec2{x, y}, 2);
    return  (int) z;
}

void terrain::create_block(const block_types& block_type, const vec3& position){
    block b = block(block_type, position);
    blocks[utils::Triplet(position)] = b;
    cubes.push_back(b.block_cube);
}

void terrain::create_tree(const vec3& position){
    float seed = utils::rand();
    int height = 1 + (int) 4 * seed;
    int width = 2 + (int) 4 * seed;

    // trunk
    for (int i = 0; i < height; i++)
        create_block(block_types::wood, position + i * vec3{0,0,1});

    // leaves
    for(int z = height; z <= height + width ; z++){
        for(int x = -width; x <= width; x++){
            for(int y = -width; y <= width; y++){
                if(
                    round(utils::distance({x, y}, {0, 0})) > width - (z - height) - 1 // from width to zero
                ) continue;
                if(check_has_block(utils::Triplet(position + vec3{x, y, z}))) continue;
                create_block(block_types::leaf, position + vec3{x, y, z});
            }
        }
    }
}

bool terrain::check_has_block(const utils::Triplet& t){
    return ! (blocks.find(t) == blocks.end());
}

terrain::terrain(){

    // generate gaussians
    gaussians.push_back({4, {1, 5}, 5*vec2{1, 4}});
    gaussians.push_back({-3, {-6, 2}, 6*vec2{3, 2}});

    // initializes the blocks for the terrain
    for (int x = xmin; x < xmax; x++)
    for (int y = ymin; y < ymax; y++){

        int height = generator_function(x, y);

        for (int z = zmin; z < height; z++)
            create_block(block_types::rock, {x, y, z});

        create_block(block_types::earth, {x, y, height});

        if(utils::rand() < 0.005)
            create_tree({x, y, height + 1});
    }

    using namespace utils;
    for (auto& [pos, blk] : blocks){
        std::vector<directions> render_dirs;
        if(!check_has_block(pos + Triplet( 1, 0, 0))) render_dirs.push_back(directions::kFront);
        if(!check_has_block(pos + Triplet(-1, 0, 0))) render_dirs.push_back(directions::kBack);
        if(!check_has_block(pos + Triplet(0,  1, 0))) render_dirs.push_back(directions::kLeft);
        if(!check_has_block(pos + Triplet(0, -1, 0))) render_dirs.push_back(directions::kRight);
        if(!check_has_block(pos + Triplet(0, 0,  1))) render_dirs.push_back(directions::kTop);
        if(!check_has_block(pos + Triplet(0, 0, -1))) render_dirs.push_back(directions::kBottom);
        blk.render_directions = render_dirs;
    }
}

terrain::~terrain(){
    // dont really know how to do it
}

void terrain::draw(const environment_structure& env, bool wireframe, const vec3& player_position, const vec3& player_looking_at, const float& max_depth){
    // auto start = std::chrono::steady_clock::now();

    // blocking: fps drop
    for (auto& [pos, blk] : blocks){

        if(blk.is_being_seen(player_position, player_looking_at, max_depth))
            blk.draw(env, wireframe);
    }


    // auto end = std::chrono::steady_clock::now();
    // std::chrono::duration<double> elapsed_seconds = end-start;
    // std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

}

const std::vector<cube> terrain::get_cubes(){
    return cubes;
}