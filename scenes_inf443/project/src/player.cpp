#include <utility>
#include "player.hpp"
#include "utils.hpp"
#include "constants.hpp"

player::player(camera_controller_custom &cam, vec3 center, bool* creative)
    : creative(creative), character(center)
{
    camera = &cam;
    camera->set_position(center);
}

player::player()
{}

vec3 player::looking_at(){
    return camera->camera_model.front();
}

void player::move(const std::vector<cube>& cubes)
{
    auto inputs = camera->inputs;
    float const dt = inputs->time_interval;
    float const step = camera->keyboard_sensitivity * dt;

    // initialize move direction
    vec2 move_direction = {0, 0};
    float z_move = 0;

    // get input to move
    if(*creative){
        if (inputs->keyboard.is_pressed(GLFW_KEY_SPACE) )
            z_move += step;
        if (inputs->keyboard.is_pressed(GLFW_KEY_Q) )
            z_move -= step;
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
        z_move += velocity.z * dt - 0.5 * gravity * dt * dt;
        velocity.z -= gravity * dt;
    }

    // check for cubes colisions
    for (cube c : cubes)
    {
        // plane collision
        if (!(
            c.top() <= bottom() ||
            c.bottom() >= top()
        )){ // checks only on the strip
            auto direction = body.get_colision_direction(c);
            int axis = direction.first;
            int semiaxis = direction.second;
            if (
                !(axis == -1 || axis == 2) &&
                (semiaxis * move_direction[axis] > 0)
            ) move_direction[axis] = 0;
        }

        // z colision
        if(body.distancexy(c) < Length){ // check only on the cilinder
            if(
                bottom() >= c.top() &&
                bottom() + z_move <= c.top()
            ) {
                z_move = c.top() - bottom();
                velocity.z = 0;
                is_jumping = false;
            };
            if(
                top() <= c.bottom() &&
                top() + z_move >= c.bottom()
            ) {
                z_move = c.bottom() - top() ;
                velocity.z = 0;
            }
        }

    }

    // updates position
    position += utils::expand(move_direction);
    position.z += z_move;

    // moves!
    legs.position = position;

    body.position = position;
    body.position.z += Length;

    camera->set_position(body.position);
}
