#include <utility>
#include "mob.hpp"
#include "utils.hpp"
#include "constants.hpp"
#include "block.hpp"

mob::mob(vec3 center) : character(center){
}

mob::mob() : character() {
}


void mob::draw(const environment_structure& env, block_mesh mesh){
    mesh.draw_all(env, position, false);
}


void mob::move(const std::vector<cube>& cubes, vec3 &position_player, const float &dt) 
{
    float move_sensibility = 3.0f;
    float const step = move_sensibility * dt;

    // initialize move direction
    vec2 move_direction = {0, 0};
    float z_move = 0;

    // move direction to player position
    move_direction += step * utils::standardize_direction(position_player - position);

    // updating z on gravity
    z_move += velocity.z * dt - 0.5 * gravity * dt * dt;
    velocity.z -= gravity * dt;

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
                !(axis == -1 || axis == 2)
            ) {
                if(semiaxis * move_direction[axis] > 0) {
                    move_direction[axis] = 0; //TODO MOB HAS TO JUMP IF COLLISION
                }
                if(!is_jumping) {
                    velocity += {0, 0, jump_velocity};
                    is_jumping = true;
                   }
                std::cout << is_jumping  << " " <<dt << std::endl;
                }
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

    // camera->set_position(body.position);
}
