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

public:
    bool* creative;
    camera_controller_custom* camera;

    player(camera_controller_custom&, vec3 center, bool* creative, terrain* terr);
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

    void start_game();
    void end_game();

    std::string get_block(int direction = 0);
};