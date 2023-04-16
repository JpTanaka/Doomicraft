#include "camera.hpp"

camera_controller_custom::camera_controller_custom()
{
    camera_model.axis_of_rotation = {0, 0, 1};
    camera_model.position_camera = {0, 0, 5};
}

void camera_controller_custom::set_position(vec3& position){
    camera_model.position_camera = position;
}

void camera_controller_custom::action_mouse_move(mat4 &camera_matrix_view)
{
    // Preconditions
    assert_cgp_no_msg(inputs != nullptr);
    assert_cgp_no_msg(window != nullptr);

    if (cursor_enabled)
        return;

    glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    vec2 const &p1 = inputs->mouse.position.current;
    vec2 const &p0 = inputs->mouse.position.previous;
    vec2 const dp = mouse_sensitivity * (p1 - p0);

    bool const event_valid = !inputs->mouse.on_gui;
    // bool const click_left = inputs->mouse.click.left;
    // bool const click_right = inputs->mouse.click.right;

    auto sign = [](float x)
    { return x < 0 ? -1 : 1; };

    if (event_valid)
    {
        camera_model.manipulator_rotate_roll_pitch_yaw(0,
                                                       std::abs(camera_model.pitch) <= pi / 2.0 ? dp.y : -0.001 * sign(camera_model.pitch),
                                                       -dp.x);
    }

    update(camera_matrix_view);
}

void camera_controller_custom::action_keyboard(mat4 &)
{
    // Escape toggles "menu mode"
    if (inputs->keyboard.last_action.is_pressed(GLFW_KEY_ESCAPE) && !cursor_enabled)
    {
        glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        cursor_enabled = true;
    }
}

void camera_controller_custom::idle_frame(mat4 &camera_matrix_view)
{
    // Preconditions
    assert_cgp_no_msg(inputs != nullptr);
    assert_cgp_no_msg(window != nullptr);
    if (!is_active)
        return;

    float const magnitude = keyboard_sensitivity * inputs->time_interval;

    // displacement with WSAD

    update(camera_matrix_view);
}