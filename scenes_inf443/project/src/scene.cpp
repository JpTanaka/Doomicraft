#include "scene.hpp"


using namespace cgp;

void scene_structure::initialize()
{
	camera_control.initialize(inputs, window); 
	environment.light = {50, 50, 50};
	glfwSetCursorPos(window.glfw_window, 0, 0);

	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	block::initialize();
	terr = terrain();
	player = character(camera_control, {2, 0, 10}, false);

}


void scene_structure::display_frame()
{


	timer.update();

	terr.draw(environment, player.get_eyes());

}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);

}

void scene_structure::mouse_move_event()
{
	if (!inputs.keyboard.shift)
		camera_control.action_mouse_move(environment.camera_view);
}
void scene_structure::mouse_click_event()
{
	camera_control.action_mouse_click(environment.camera_view);
}
void scene_structure::keyboard_event()
{
	camera_control.action_keyboard(environment.camera_view);
}
void scene_structure::idle_frame()
{
	camera_control.idle_frame(environment.camera_view);
	player.move(terr.get_cubes());
}
