#include "scene.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char *filename, GLuint *out_texture, int *out_width, int *out_height)
{
	// Load from file
	int image_width = 0;
	int image_height = 0;
	unsigned char *image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	return true;
}

using namespace cgp;

void scene_structure::initialize()
{
	camera_control.initialize(inputs, window);
	environment.light = {50, 50, 50};
	glfwSetCursorPos(window.glfw_window, 0, 0);

	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	block::initialize();
	terr = terrain();
	main_player = player(camera_control, {2, 0, 10}, &gui.creative);

	block b = block(block_types::rock, {0, 0, 15});

	// Adding portal gun
	glfwInit();
	bool ret = LoadTextureFromFile("../assets/portal_gun.png", &gui.portal_gun.image_texture, &gui.portal_gun.image_width, &gui.portal_gun.image_height);
	IM_ASSERT(ret);
	ret = LoadTextureFromFile("../assets/crosshair.png", &gui.crosshair.image_texture, &gui.crosshair.image_width, &gui.crosshair.image_height);
	IM_ASSERT(ret);
}

void scene_structure::display_frame()
{
	timer.update();
	terr.draw(environment, gui.display_wireframe);
}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);
	ImGui::Checkbox("Creative", &gui.creative);
	
	bool exit = ImGui::Button("Exit");
	if(exit) {
		std::exit(0);
	}
	ImGui::Begin("Weapon", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration);
	gui.portal_gun.image_height = std::min(window.width / 4.0f, 300.0f);
	gui.portal_gun.image_width = std::min(window.width / 4.0f, 300.0f);
	ImGui::SetWindowPos({(float)window.width - (float)gui.portal_gun.image_width, (float)window.height - (float)gui.portal_gun.image_height});
	ImGui::SetWindowSize({(float)gui.portal_gun.image_width, (float)gui.portal_gun.image_height});
	ImGui::Image((void *)(intptr_t)gui.portal_gun.image_texture, ImVec2(gui.portal_gun.image_width, gui.portal_gun.image_height));
	ImGui::End();

	ImGui::Begin("Crosshair", NULL, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration);
	gui.crosshair.image_height = std::min(30.0f, window.width / 30.0f);
	gui.crosshair.image_width = std::min(30.0f, window.width / 30.0f);
	ImGui::SetWindowPos({((float)window.width - (float)gui.crosshair.image_width) / 2.0f,
	((float)window.height - (float)gui.crosshair.image_height) / 2.0f});
	ImGui::Image((void *)(intptr_t)gui.crosshair.image_texture, ImVec2((float)gui.crosshair.image_width, (float)gui.crosshair.image_height));
	ImGui::End();
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
	main_player.move(terr.get_cubes());
}
