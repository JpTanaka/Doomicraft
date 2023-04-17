#include <utility>
#include "character.hpp"
#include "utils.hpp"

character::character(camera_controller_custom &cam, vec3 center)
    : body(center)
{
    camera = &cam;
    position = center;
    direction = {1.0f, 0.0f};
    velocity = {0, 0, 0};

    camera->set_position(center);
    is_jumping = false;
}

character::character() {}

void character::move(std::vector<cube> cubes)
{
    auto inputs = camera->inputs;
    float const dt = inputs->time_interval;
    float const step = camera->keyboard_sensitivity * dt;

    // initialize move direction
    vec2 move_direction = {0, 0};
    float z_move = 0;

    // get input to move
    if (inputs->keyboard.is_pressed(GLFW_KEY_SPACE) && !is_jumping){
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
    z_move += velocity.z * dt - 0.5 * gravity * dt * dt;
    velocity.z -= gravity * dt;

    // check for cubes colisions
    for (cube c : cubes)
    {
        // plane collision
        if (!(
            c.top() <= body.bottom() ||
            c.bottom() >= body.top()
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
        if(body.distancexy(c) < body.L){ // check only on the cilinder
            if(
                body.bottom() >= c.top() &&
                body.bottom() + z_move <= c.top()
            ) {
                z_move = c.top() - body.bottom();
                velocity.z = 0;
                is_jumping = false;
            };
            if(
                body.top() <= c.bottom() &&
                body.top() + z_move >= c.bottom()
            ) z_move = c.bottom() - body.top() ;
        }

    }

    // updates position
    position += utils::expand(move_direction);
    position.z += z_move;

    // moves!
    camera->set_position(position);
    body.position = position;

}
