#include <utility>
#include "mob.hpp"
#include "utils.hpp"
#include "constants.hpp"
#include "block.hpp"

mob::mob(vec3 center) : character(center){
    float l = Length;
    mesh.initialize_data_on_gpu(
        mesh_primitive_quadrangle(
            { l/2.0, l/2.0,-l/2.0},
            { l/2.0,-l/2.0,-l/2.0},
            { l/2.0,-l/2.0, l*3/2.0-0.2},
            { l/2.0, l/2.0, l*3/2.0-0.2}
        )
    );
    mesh.texture.load_and_initialize_texture_2d_on_gpu( 
        project::path + "assets/creeper_pirate.png"
    );
    health_bar.initialize_data_on_gpu(
        mesh_primitive_quadrangle(
            { l/2.0, l/2.0, l*3/2.0 - 0.03},
            { l/2.0,-l/2.0, l*3/2.0 - 0.03},
            { l/2.0,-l/2.0, l*3/2.0 + 0.03},
            { l/2.0, l/2.0, l*3/2.0 + 0.03}
        )
    );
    health_bar.material.color = {0, 1, 0};
}

mob::mob() : character() {}

void mob::draw(const environment_structure& env,bool wireframe){
    mesh.model.translation = position;
    health_bar.model.translation = position;
    if(!wireframe) {
        cgp::draw(mesh, env);
        cgp::draw(health_bar, env);
    }
    else {
        cgp::draw_wireframe(mesh, env);
        cgp::draw_wireframe(health_bar, env);
    }
}

void mob::take_damage(){
    life--;
    mesh.material.color -= vec3{0, 1, 1} * 1/(float)max_life;
    health_bar.model.scaling_xyz = {1, (life)/(float)max_life, 1};
}

bool mob::is_dead(){
    return life <= 0;
}

void mob::move(
    const std::vector<cube>& cubes, 
    vec3 &position_player, const float &dt, 
    const float& move_sensibility
) 
{
    float const step = move_sensibility * dt;

    // initialize move direction
    vec2 move_direction = {0, 0};
    float z_move = 0;

    // move direction to player position
    move_direction += step * utils::standardize_direction(position_player+vec3{0.1,0.1,0.1} - position);
    if(norm(move_direction_xy)>0.01){
        mesh.model.rotation = rotation_transform::from_frame_transform({1,0,0}, {0,0,1}, normalize(vec3{move_direction.x, move_direction.y, 0}), {0,0,1});
        health_bar.model.rotation = rotation_transform::from_frame_transform({1,0,0}, {0,0,1}, normalize(vec3{move_direction.x, move_direction.y, 0}), {0,0,1});
    }
    if(norm(move_direction)>0.01) 
        move_direction_xy = normalize(utils::expand({move_direction.x, move_direction.y}));

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
                    move_direction[axis] = 0; 
                }
                if(!is_jumping) {
                    velocity += {0, 0, jump_velocity};
                    is_jumping = true;
                    break;
                   }
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
}
