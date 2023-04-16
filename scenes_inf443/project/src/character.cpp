#include <utility>
#include "character.hpp"


character::character(camera_controller_custom& cam, vec3 center)
    :body(center)
{
    camera = &cam;
    position = center;
    direction = {1.0f, 0.0f};

    camera->set_position(center);
    velocity_z = 0;
    is_jumping = false;
}

character::character(){}

void character::move(std::vector<cube> cubes){
    mat3 plan_projection = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 0},
    };
    auto inputs = camera->inputs;
    float const magnitude = camera->keyboard_sensitivity * inputs->time_interval;

    // encoding possible directions of colision
    float   xpos = 1000, 
            ypos = 1000, 
            zpos = 1000, 
            xneg = 1000, 
            yneg = 1000, 
            zneg = 1000;

    for (auto c : cubes){
        vec3 colision = c.colision(body);
        if (colision.x > 0) xpos = std::min(xpos, std::abs(colision.x));
        if (colision.y > 0) ypos = std::min(ypos, std::abs(colision.y));
        if (colision.z > 0) zpos = std::min(zpos, std::abs(colision.z));
        if (colision.x < 0) xneg = std::min(xneg, std::abs(colision.x));
        if (colision.y < 0) yneg = std::min(yneg, std::abs(colision.y));
        if (colision.z < 0) zneg = std::min(zneg, std::abs(colision.z));
    }


    float eps = 0.1;
    auto colide = [&](vec3 dir){
        if (xpos < body.L + eps && dir.x < 0) dir.x = 0;
        if (xneg < body.L + eps && dir.x > 0) dir.x = 0;
        if (ypos < body.L + eps && dir.y < 0) dir.y = 0;
        if (yneg < body.L + eps && dir.y > 0) dir.y = 0;
        if (zneg < body.L + eps && dir.z > 0) dir.z = 0;
        if (zpos < body.L + eps && dir.z < 0){
            is_jumping = false;
            velocity_z = 0;
            dir.z = 0;
        }
        return dir;
    };


    float dt = 0.015;
    float dz = velocity_z*dt - 0.5 * gravity * dt * dt;
    velocity_z += - gravity * dt;
    position += colide(vec3{0,0,1} * dz);

    if (inputs->keyboard.is_pressed(GLFW_KEY_SPACE) && !is_jumping){
        is_jumping = true;
        velocity_z = 6;
    }

    // if (inputs->keyboard.shift)
    //     position += magnitude * colide({0,0,-1});

    if (inputs->keyboard.is_pressed(GLFW_KEY_D))
        position += magnitude * colide(normalize(plan_projection * camera->camera_model.right()));
    if (inputs->keyboard.is_pressed(GLFW_KEY_A))
        position += magnitude * colide(-normalize(plan_projection * camera->camera_model.right()));
    if (inputs->keyboard.is_pressed(GLFW_KEY_W))
        position += magnitude * colide(normalize(plan_projection * camera->camera_model.front()));
    if (inputs->keyboard.is_pressed(GLFW_KEY_S))
        position += magnitude * colide(-normalize(plan_projection * camera->camera_model.front()));

    camera->set_position(position);
    body.position = position;
}


