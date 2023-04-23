#include "camera.hpp"
#include "utils.hpp"

camera_controller_custom::camera_controller_custom()
{
    camera_model.axis_of_rotation = {0, 0, 1};
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

    if (event_valid)
    {
        camera_model.manipulator_rotate_roll_pitch_yaw(
            0,
            dp.y > 0 
                ? std::min(dp.y, 0.5f*pi - camera_model.pitch  - 0.01f)
                : std::max(dp.y, -0.5f*pi - camera_model.pitch + 0.01f),
            -dp.x
        );
    }

    update(camera_matrix_view);
}


void camera_controller_custom::deactivate(){
    glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    cursor_enabled = true;
}

void camera_controller_custom::action_keyboard(mat4 &, bool& config_mode)
{
    // Escape toggles "menu mode"
    if (inputs->keyboard.last_action.is_pressed(GLFW_KEY_ESCAPE) )
    {
        glfwSetInputMode(window->glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        cursor_enabled = !cursor_enabled;
		config_mode = !config_mode;
    }
}

void camera_controller_custom::idle_frame(mat4 &camera_matrix_view)
{
    // Preconditions
    assert_cgp_no_msg(inputs != nullptr);
    assert_cgp_no_msg(window != nullptr);
    if (!is_active)
        return;
    update(camera_matrix_view);
}