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
    std::optional<block_types> chosen_block; // if empty means that you are breaking blocks ;)
    void shoot_mob(mob_group &mobg);
    void handle_cubes(const std::vector<cube>& cubes);

    int kills = 0;

public:
    bool* creative;
    camera_controller_custom* camera;

    player(camera_controller_custom&, vec3 center, bool* creative, terrain* terr);
    player();

    void move(const std::vector<cube>&);
    vec3 looking_at();

    float detect_colision (std::vector<cube>, float max_distance);

    void handle_keyboard_input();
    void handle_mouse_input(const std::vector<cube>& cubes, mob_group &mobg);

    int get_kills();
};