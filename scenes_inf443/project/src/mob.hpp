#pragma once

#include "cgp/cgp.hpp"
#include "camera.hpp"
#include "cube.hpp"
#include "constants.hpp"
#include "character.hpp"
#include "block.hpp"
#include "utils.hpp"

class mob : public character {
public:
    int life = 4;
    mob(vec3 center);
    mob();

    void move(const std::vector<cube>&, vec3&, const float &);
    void draw(const environment_structure& env, block_mesh);

};