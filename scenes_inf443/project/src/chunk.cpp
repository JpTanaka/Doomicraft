#include "chunk.hpp"

chunk::chunk(){};

// spawn some threads to do this pleaseeeeee
chunk::chunk(vec2 chunk_position)
    :position({
        chunk_position.x * chunk_x_size, 
        chunk_position.y * chunk_y_size
    })
{

    for (int x = 0; x < chunk_x_size; x++)
    for (int y = 0; y < chunk_y_size; y++){

        int height = generator_function({x, y});

        for (int z = -chunk_z_size; z < height; z++)
            create_block(block_types::rock, {x, y, z});

        create_block(block_types::earth, {x, y, height});

        if(utils::rand() < 0.005)
            create_tree({x, y, height + 1});
    }

    for (auto& [pos, blk] : blocks){
        std::vector<directions> render_dirs;
        if(!check_has_block(pos + utils::Triplet( 1, 0, 0))) render_dirs.push_back(directions::kFront);
        if(!check_has_block(pos + utils::Triplet(-1, 0, 0))) render_dirs.push_back(directions::kBack);
        if(!check_has_block(pos + utils::Triplet(0,  1, 0))) render_dirs.push_back(directions::kLeft);
        if(!check_has_block(pos + utils::Triplet(0, -1, 0))) render_dirs.push_back(directions::kRight);
        if(!check_has_block(pos + utils::Triplet(0, 0,  1))) render_dirs.push_back(directions::kTop);
        if(!check_has_block(pos + utils::Triplet(0, 0, -1))) render_dirs.push_back(directions::kBottom);
        blk.render_directions = render_dirs;
    }
}

int chunk::generator_function(const vec2& v){
    double z = 0;
    z += 3.0f * cgp::noise_perlin(0.05*(position + v), 1);
    z += 0.5f * cgp::noise_perlin(0.05*(position + v), 2);
    return  (int) z;
}
    
void chunk::create_tree(const vec3& center){
    float seed = utils::rand();
    int height = 1 + (int) 4 * seed;
    int width = 2 + (int) 4 * seed;

    // trunk
    for (int i = 0; i < height; i++)
        create_block(block_types::wood, center + i * vec3{0,0,1});

    // leaves
    for(int z = height; z <= height + width ; z++){
        for(int x = -width; x <= width; x++){
            for(int y = -width; y <= width; y++){
                if(
                    round(utils::distance({x, y}, {0, 0})) > width - (z - height) - 1 // from width to zero
                ) continue;
                if(check_has_block(utils::Triplet(center + vec3{x, y, z}))) continue;
                create_block(block_types::leaf, center + vec3{x, y, z});
            }
        }
    }
}

bool chunk::check_has_block(const utils::Triplet& t){
    return ! (blocks.find(t) == blocks.end());
}

void chunk::create_block(const block_types& block_type, const vec3& center){
    block b = block(block_type, utils::expand(position) + center);
    blocks[utils::Triplet(b.position)] = b;
    cubes.push_back(b.block_cube);
}

void chunk::draw(const environment_structure& env, bool wireframe, const vec3& player_position, const vec3& player_looking_at, const float& max_depth){
    for (auto& [pos, blk] : blocks){
        if(blk.is_being_seen(player_position, player_looking_at, max_depth))
            blk.draw(env, wireframe);
    }
}
    
const std::vector<cube> chunk::get_cubes(){
    return cubes;
}