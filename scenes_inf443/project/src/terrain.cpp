#include "terrain.hpp"
#include "utils.hpp"

int terrain::generator_function(int x, int y){

    double z = 0;
    for (auto tup : gaussians){
        z += get<0>(tup) * utils::gaussian({x,y}, get<1>(tup), get<2>(tup));
    }
    z += 3.0f * cgp::noise_perlin(0.02*vec2{x, y}, 1);
    z += 0.5f * cgp::noise_perlin(0.05*vec2{x, y}, 2);
    return  (int) z;
}

terrain::terrain(){

    // generate gaussians
    gaussians.push_back({4, {1, 5}, 5*vec2{1, 4}});
    gaussians.push_back({-3, {-6, 2}, 6*vec2{3, 2}});

    // initializes the blocks for the terrain
    for (int x = xmin; x < xmax; x++)
    for (int y = ymin; y < ymax; y++){

        int height = generator_function(x, y);
        block b;

        for (int z = zmin; z < height; z++){
            b = block(block_types::rock, {x, y, z });
            utils::Triplet t(x, y, z);
            blocks[utils::Triplet(x, y, z)] = b;
            // blocks.push_back(b);
            cubes.push_back(b.block_cube);
        }
        b = block(block_types::earth, {x, y, height });
        // blocks.push_back(b);
        blocks[utils::Triplet(x, y, height)] = b;
        cubes.push_back(b.block_cube);
    }
}

terrain::~terrain(){
    // dont really know how to do it
}

void terrain::draw(environment_structure& env, bool wireframe){
    using namespace utils;

    auto check_has_block = [&](Triplet t){
        return ! (blocks.find(t) == blocks.end());
    };

    // blocking: fps drop
    for (auto& [pos, blk] : blocks){
        std::vector<directions> render_dirs;
        if(!check_has_block(pos + Triplet( 1, 0, 0))) render_dirs.push_back(directions::kFront);
        if(!check_has_block(pos + Triplet(-1, 0, 0))) render_dirs.push_back(directions::kBack);
        if(!check_has_block(pos + Triplet(0,  1, 0))) render_dirs.push_back(directions::kLeft);
        if(!check_has_block(pos + Triplet(0, -1, 0))) render_dirs.push_back(directions::kRight);
        if(!check_has_block(pos + Triplet(0, 0,  1))) render_dirs.push_back(directions::kTop);
        if(!check_has_block(pos + Triplet(0, 0, -1))) render_dirs.push_back(directions::kBottom);
        blk.draw(env, render_dirs, wireframe);
    }
}

std::vector<cube> terrain::get_cubes(){
    return cubes;
}