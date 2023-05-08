#pragma once

#include <optional>
#include "cgp/cgp.hpp"
#include "camera.hpp"
#include "cube.hpp"
#include "constants.hpp"
#include "character.hpp"
#include "mob_group.hpp"
#include "terrain.hpp"

class player : public character {

    terrain* terr = nullptr;
    block_types chosen_block = block_types::earth;
    bool shoot_mob(mob_group &mobg);
    void handle_cubes(const std::vector<cube>& cubes);

    int kills = 0;
    int health = 5;
    int max_health = 5;
    vec3 respawn_delta = {50, 0, 10};
    int respawn_time = 100;
    int respawn_timer = 1000;

    vec3 starting_position;
    bool game_on = false;

    void respawn();

    // new collision system
    vec3 collide(const std::vector<cube>& cubes, const vec3& move_direction);
    vec3 center = {0, 0, 0};
    vec3 dimensions = vec3{0.7, 0.7, 2 * 0.9} * Length;
    float top();
    float bottom();
    bool is_inside(const vec3&) const;

    vec3 camera_pos;

    environment_structure* p_env;

public:
    // collision debugging
    mesh_drawable collision_box;
    void draw_collision_box(environment_structure&);

    bool* creative;
    camera_controller_custom* camera;

    player(camera_controller_custom&, vec3 center, bool* creative, terrain* terr, environment_structure* p_env = nullptr);
    player();

    void move(const std::vector<cube>&);
    vec3 looking_at();

    float detect_colision (std::vector<cube>, float max_distance);

    void handle_keyboard_input();
    bool handle_mouse_input(const std::vector<cube>& cubes, mob_group &mobg); // returns true if mob was hit

    int get_kills();
    void take_hit();
    int get_health();
    int get_max_health();
    bool is_dead();
    int get_level();
    vec3 get_eyes();
    void start_game();
    void end_game();

    std::string get_block(int direction = 0);
};