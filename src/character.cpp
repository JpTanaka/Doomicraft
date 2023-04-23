#include <utility>
#include "character.hpp"
#include "utils.hpp"
#include "constants.hpp"

character::character(vec3 center)
{
    position = center;
    direction = {1.0f, 0.0f};
    velocity = {0, 0, 0};
    body = cube(center);
    body.position.z += Length;
    legs = cube(center);
    is_jumping = false;
}



character::character()
{

}

float character::bottom(){
    return legs.bottom();
}

float character::top(){
    return body.top();
}

vec3 character::get_eyes(){
    return body.position;
}
