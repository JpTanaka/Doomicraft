#include "character.hpp"


character::character(camera_controller_custom& cam, vec3 center)
    :body(center)
{
    camera = &cam;
    position = center;
    direction = {1.0f, 0.0f};

    camera->set_position(center);
}

character::character(){}

void character::move(){
    mat3 plan_projection = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 0},
    };
    auto inputs = camera->inputs;
    float const magnitude = camera->keyboard_sensitivity * inputs->time_interval;


    if (inputs->keyboard.is_pressed(GLFW_KEY_SPACE))
        position.z += magnitude;
    if (inputs->keyboard.shift)
        position.z -= magnitude;

    if (inputs->keyboard.is_pressed(GLFW_KEY_D))
        position += magnitude * normalize(plan_projection * camera->camera_model.right());
    if (inputs->keyboard.is_pressed(GLFW_KEY_A))
        position -= magnitude * normalize(plan_projection * camera->camera_model.right());
    if (inputs->keyboard.is_pressed(GLFW_KEY_W))
        position += magnitude * normalize(plan_projection * camera->camera_model.front());
    if (inputs->keyboard.is_pressed(GLFW_KEY_S))
        position -= magnitude * normalize(plan_projection * camera->camera_model.front());

    camera->set_position(position);
    body.position = position;
}


