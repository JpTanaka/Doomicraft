#include <utility>
#include "player.hpp"
#include "utils.hpp"
#include "constants.hpp"
#include "audio_controller.hpp"

player::player(camera_controller_custom &cam, vec3 center, bool* creative, terrain* terr)
    : character(center), terr(terr), creative(creative)
{
    this->center = center + vec3{0, 0, 0.5 * dimensions.z};
    collision_box.initialize_data_on_gpu(mesh_primitive_cube());
    collision_box.model.scaling_xyz = dimensions;

    camera = &cam;
    camera->set_position(center);
    starting_position = center;
}

player::player()
{}

vec3 player::looking_at(){
    return camera->camera_model.front();
}

vec3 player::collide(const std::vector<cube>& cubes, const vec3& move_direction){

    return move_direction;

    // vec3 move = move_direction;
    // float z_move = move.z;
    // for (cube c : cubes){
    //     // plane collision
    //     if (!(
    //         c.top() <= bottom() ||
    //         c.bottom() >= top()
    //     )){ // checks only on the strip
    //         auto direction = body.get_colision_direction(c);
    //         int axis = direction.first;
    //         int semiaxis = direction.second;
    //         if (
    //             !(axis == -1 || axis == 2) &&
    //             (semiaxis * move_direction[axis] > 0)
    //         ) move[axis] = 0;
    //     }

    //     // z colision
    //     if(body.distancexy(c) < Length){ // check only on the cilinder
    //         if(
    //             bottom() >= c.top() &&
    //             bottom() + z_move <= c.top()
    //         ) {
    //             z_move = c.top() - bottom();
    //             velocity.z = 0;
    //             is_jumping = false;
    //         };
    //         if(
    //             top() <= c.bottom() &&
    //             top() + z_move >= c.bottom()
    //         ) {
    //             z_move = c.bottom() - top() ;
    //             velocity.z = 0;
    //         }
    //     }
    // }
    // return move;
}

void player::move(const std::vector<cube>& cubes)
{
    if (respawn_timer < respawn_time){
        respawn_timer++;
        return;
    }
    auto& inputs = camera->inputs;
    float const dt = inputs->time_interval;
    float const step = camera->keyboard_sensitivity * dt;

    // initialize move direction
    vec3 move_direction = {0, 0, 0};

    // get input to move
    if(*creative){
        if (inputs->keyboard.is_pressed(GLFW_KEY_SPACE) )
            move_direction.z += step;
        if (inputs->keyboard.is_pressed(GLFW_KEY_Z) )
            move_direction.z -= step;
    }

    if (inputs->keyboard.is_pressed(GLFW_KEY_SPACE) && !is_jumping && !*creative){
        velocity += {0, 0, jump_velocity};
        is_jumping = true;
    }

    if (inputs->keyboard.is_pressed(GLFW_KEY_D))
        move_direction += step * utils::standardize_direction(camera->camera_model.right());

    if (inputs->keyboard.is_pressed(GLFW_KEY_A))
        move_direction += -step * utils::standardize_direction(camera->camera_model.right());

    if (inputs->keyboard.is_pressed(GLFW_KEY_W))
        move_direction += step * utils::standardize_direction(camera->camera_model.front());

    if (inputs->keyboard.is_pressed(GLFW_KEY_S))
        move_direction += -step * utils::standardize_direction(camera->camera_model.front());

    // updating z on gravity
    if(!*creative){
        move_direction.z += velocity.z * dt - 0.5 * gravity * dt * dt;
        velocity.z -= gravity * dt;
    }

    // check for cubes colisions
    move_direction = collide(cubes, move_direction);

    // updates position
    position += move_direction;
    center += move_direction;

    // moves!
    legs.position = position;

    body.position = position;
    body.position.z += Length;

    camera->set_position(body.position);
}


float player::detect_colision (std::vector<cube> cubes, float max_distance){
    float distance = INFTY;

    for (const cube& c: cubes){
        if (norm(c.position - get_eyes()) >= max_distance) continue;
        distance = std::min(
            distance,
            utils::distance_point_cube(
                get_eyes(),
                looking_at(),
                c.position,
                Length
            )
        );
    }
    return distance;
}

void player::handle_keyboard_input(){
    auto& inputs = camera->inputs;
    if (inputs->keyboard.is_pressed(GLFW_KEY_Q)) chosen_block = block::get_next_block(chosen_block, -1);
    if (inputs->keyboard.is_pressed(GLFW_KEY_E)) chosen_block = block::get_next_block(chosen_block, 1);
    if (inputs->keyboard.is_pressed(GLFW_KEY_1)) chosen_block = block_types::NO_BLOCK;
}

bool player::handle_mouse_input(const std::vector<cube>& cubes, mob_group &mobg){
    auto& inputs = camera->inputs;
    bool const click_right = inputs->mouse.click.right;
    bool const click_left = inputs->mouse.click.left;

    if (click_right)
        handle_cubes(cubes);

    if (click_left){
        bool hit = shoot_mob(mobg);
        if(game_on)
            lists.shoot = true;
        return hit;
    }
    return false;
}


void player::handle_cubes(const std::vector<cube>& cubes){
    float dist = detect_colision(cubes, 5);
    if (dist == INFTY) return;

    vec3 where = get_eyes() + 
        looking_at() * (dist + kEps * (chosen_block == block_types::NO_BLOCK ? 1 : -1));

    // check if inside player
    if (
        utils::distance(where, get_eyes()) < Length ||
        utils::distance(where, get_legs()) < Length 
    ) return;

    if (chosen_block == block_types::NO_BLOCK)
        terr->delete_bloc(where);
    else
        terr->create_bloc(where, chosen_block);
}

bool player::shoot_mob(mob_group &mobg) {
    bool hit = mobg.shoot_mob(
        get_eyes(),
        looking_at(), 
        detect_colision(mobg.get_cubes(), 20.0f)
    );
    int new_kills = mobg.check_dead(position);
    kills += new_kills;
    if (new_kills > 0) lists.kill = true;
    return hit;
}

int player::get_kills(){
    return kills;
}

int player::get_health(){
    return health;
}

int player::get_max_health(){
    return max_health;
}

void player::take_hit(){
    health--;
    respawn();
}

void player::respawn(){
    lists.death = true;
    respawn_timer = 0;

    position += respawn_delta;
    legs.position += respawn_delta;
    body.position += respawn_delta;
    camera->set_position(body.position);
}

bool player::is_dead(){
    return health <= 0;
}

int player::get_level(){
    return kills / 10;
}

std::string player::get_block(int direction){
    
    switch (
        direction ==  0 ? chosen_block : 
        direction == -1 ? block::get_next_block(chosen_block, -1) :
        direction ==  1 ? block::get_next_block(chosen_block,  1) :
        chosen_block
    ){
    case block_types::earth:
        return "Earth";
    case block_types::rock:
        return "Rock";
    case block_types::wood:
        return "Wood";
    case block_types::leaf:
        return "Leaves";
    case block_types::wood_plank:
        return "Wood Plank";
    case block_types::brick:
        return "Brick";
    case block_types::sand:
        return "Sand";
    case block_types::stone_brick:
        return "Stone Brick";
    case block_types::NO_BLOCK:
        return "Pickaxe";
    default:
        return "";
    }
    return "";
}


void player::start_game(){
    game_on = true;
}

void player::end_game(){
    game_on = false;
}

void player::draw_collision_box(environment_structure& env){
    collision_box.model.translation = center;
    draw_wireframe(collision_box, env);
}