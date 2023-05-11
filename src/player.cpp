#include <utility>
#include <limits>
#include "player.hpp"
#include "utils.hpp"
#include "constants.hpp"
#include "audio_controller.hpp"

player::player(camera_controller_custom &cam, vec3 center, bool* creative, terrain* terr, game_modes* game_mode, environment_structure* p_env)
    :character(center), terr(terr), game_mode(game_mode), p_env(p_env), creative(creative)
{
    this->center = center + vec3{0, 0, 0.5 * dimensions.z};
    collision_box.initialize_data_on_gpu(mesh_primitive_cube());
    collision_box.model.scaling_xyz = dimensions;

    gun.initialize_data_on_gpu(
        mesh_load_file_obj("../assets/gun.obj")
    );
    gun.material.color = 0.3f * vec3{1, 1, 1};

    camera = &cam;
    camera_pos = this->center + vec3{0, 0, 0.5f * dimensions.z - 0.2f};
    camera->set_position(camera_pos);
    starting_position = center;
}

player::player()
{}

vec3 player::looking_at(){
    return camera->camera_model.front();
}


float sweepAABB (
    const vec3& player_center,
    const vec3& player_dimensions,
    const vec3& cube_center,
    const vec3& cube_dimensions,
    vec3& normal,
    const vec3& move
){
    vec3 entry, exit, entry_t, exit_t;
    for (int i = 0; i < 3; i++){
        float p_start = player_center[i] - 0.5f * player_dimensions[i];
        float p_end   = player_center[i] + 0.5f * player_dimensions[i];
        float c_start = cube_center[i]   - 0.5f * cube_dimensions[i];
        float c_end   = cube_center[i]   + 0.5f * cube_dimensions[i];

        // entry and exit distances for each direction
        entry[i] = move[i] > 0.0f ? c_start - p_end : c_end - p_start;
        exit[i]  = move[i] > 0.0f ? c_end - p_start : c_start - p_end;

        if (move[i] == 0) {
            entry_t[i] = - std::numeric_limits<float>::infinity();
            exit_t[i]  =   std::numeric_limits<float>::infinity();
        }
        else {
            entry_t[i] = entry[i] / move[i];
            exit_t[i]  = exit[i]  / move[i];
        }
    }

    float entry_time = utils::max(entry_t);
    float exit_time  = utils::min(exit_t);

    if( // no collision
        entry_time > exit_time ||
        (entry_t.x < 0.0f && entry_t.y < 0.0f && entry_t.z < 0.0f)||
        (entry_t.x > 1.0f) ||
        (entry_t.y > 1.0f) ||
        (entry_t.z > 1.0f)
    ) {
        normal = {0, 0, 0};
        return 1.0f;
    }

    // collision
    if (entry_t.x > entry_t.y && entry_t.x > entry_t.z) normal = {entry.x <= 0.0f ? 1.0f : -1.0f, 0, 0};
    if (entry_t.y > entry_t.x && entry_t.y > entry_t.z) normal = {0, entry.y <= 0.0f ? 1.0f : -1.0f, 0};
    if (entry_t.z > entry_t.y && entry_t.z > entry_t.x) normal = {0, 0, entry.z <= 0.0f ? 1.0f : -1.0f};

    return entry_time;
}

bool in_box(const vec3& box_center, const vec3& box_dimension, const vec3& obj_center, const vec3& obj_dimension){
    for (int i = 0; i < 3; i ++)
        if (
            obj_center[i] - 0.5f * obj_dimension[i] >= box_center[i] + 0.5f * box_dimension[i] ||
            obj_center[i] + 0.5f * obj_dimension[i] <= box_center[i] - 0.5f * box_dimension[i]
        ) return false;
    return true;
}

vec3 player::collide(const std::vector<cube>& cubes, const vec3& move_direction){
    vec3 new_move_direction = move_direction;

    for (int i = 0; i < 3; i++){
        float t_min = 1.0f;
        vec3 col_normal = {0, 0, 0};
        vec3 box_center = center + 0.5f * new_move_direction;
        vec3 box_dimentions = utils::abs(new_move_direction) + utils::abs(dimensions);

        for (const cube& c: cubes){
            // broadphase
            const vec3& pos = c.position;
            if (!in_box(box_center, box_dimentions, pos, vec3{1, 1, 1} * Length)) continue;
            vec3 normal = {0, 0, 0};
            float t_entry = sweepAABB(center, dimensions, c.position, vec3{1, 1, 1} * Length, normal, new_move_direction);
            if (t_entry < t_min){
                t_min = t_entry; 
                col_normal = normal;
            }
            // debugging
            if (t_entry < 1.0f && t_entry > 0.0f && p_env != nullptr) c.draw_wire(*p_env);
        }

        // sliding
        float remaining_time = 1.0f - t_min;
        float dp = dot(new_move_direction, col_normal);
        vec3 remainder = {0, 0, 0};
        if (dp < 0) remainder = - dp * col_normal + new_move_direction;
        
        // updating move direction
        new_move_direction = new_move_direction * (t_min - kEps) + remainder * remaining_time;
    }
    return new_move_direction;
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

    // check will land from jump
    // not ideal implementation but it works (very simple checks and const refs)
    for (const cube& c : cubes){
        if(body.distancexy(c) < 0.5f * dimensions.x + 0.5f * Length){ // check only on the cilinder
            if(
                bottom() >= c.top() &&
                bottom() + move_direction.z <= c.top()
            ) {
                velocity.z = 0;
                is_jumping = false;
            };
            if(
                top() <= c.bottom() &&
                top() + move_direction.z >= c.bottom()
            ) velocity.z = 0;
        }
    }

    // check for cubes colisions
    move_direction = collide(cubes, move_direction);
    

    // updates position
    position += move_direction;
    center += move_direction;
    camera_pos += move_direction;

    // moves!
    legs.position = position;

    body.position = position;
    body.position.z += Length;

    camera->set_position(camera_pos);

    // moving gun in frame  
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
        if(game_on && *game_mode == game_modes::kSurvival)
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
    if (is_inside(where)) return;

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
    if (new_kills > 0 && *game_mode == game_modes::kSurvival) lists.kill = true;
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
    center += respawn_delta;
    legs.position += respawn_delta;
    body.position += respawn_delta;
    camera_pos = this->center + vec3{0, 0, 0.5f * dimensions.z - 0.2f};
    camera->set_position(camera_pos);
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

float player::top() {
    return center.z + dimensions.z * 0.5f;
}

float player::bottom() {
    return center.z - dimensions.z * 0.5f;
}

bool player::is_inside(const vec3& p) const {
    return in_box(
        center, dimensions, utils::round(p), vec3{1, 1, 1} * Length
    );
}


vec3 player::get_eyes(){
    return camera_pos;
}

void player::draw(const environment_structure& env, bool wireframe){
    if (*game_mode == game_modes::kCreative) return;
    gun.model.translation = camera->camera_model.position();
    gun.model.rotation = rotation_transform::from_frame_transform(
        {1,0,0}, 
        {0,0,1}, 
        camera->camera_model.front(), 
        camera->camera_model.up()
    );
    if (wireframe)
        cgp::draw_wireframe(gun, env);
    else
        cgp::draw(gun, env);
}