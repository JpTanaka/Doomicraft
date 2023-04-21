#pragma once

#include "cgp/cgp.hpp"
#include "camera.hpp"
#include "cube.hpp"
#include "constants.hpp"
#include "character.hpp"
#include "mob_group.hpp"
class player : public character {
public:
    bool* creative;
    camera_controller_custom* camera;

    player(camera_controller_custom&, vec3 center, bool* creative);
    player();

    void move(const std::vector<cube>&);
    vec3 looking_at();
    void shoot_mob(mob_group mobg);
    float detect_colision (const std::vector<cube>&, float max_distance);
};