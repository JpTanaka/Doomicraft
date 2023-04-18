#include "scene.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" 

// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
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

void deform_terrain(mesh& m)
{
	// Set the terrain to have a gaussian shape
	for (int k = 0; k < m.position.size(); ++k)
	{
		vec3& p = m.position[k];
		float d2 = p.x*p.x + p.y * p.y;
		float z = exp(-d2 / 4)-1;

		z = z + 0.05f*noise_perlin({ p.x,p.y });

		p = { p.x, p.y, z };
	}

	m.normal_update();
}

// This function is called only once at the beginning of the program
// This function can contain any complex operation that can be pre-computed once
void scene_structure::initialize()
{
	// Set the behavior of the camera and its initial position
	// ********************************************** //
	camera_control.initialize(inputs, window); 
	glfwSetCursorPos(window.glfw_window, 0, 0);

	// Create the global (x,y,z) frame
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());


	// Adding portal gun

	glfwInit();
	bool ret = LoadTextureFromFile("/home/tanaka/repos/Doomicraft/scenes_inf443/project/assets/portal_gun.png", &gui.portal_gun.image_texture, &gui.portal_gun.image_width, &gui.portal_gun.image_height);
	IM_ASSERT(ret);
	ret = LoadTextureFromFile("/home/tanaka/repos/Doomicraft/scenes_inf443/project/assets/crosshair.png", &gui.crosshair.image_texture, &gui.crosshair.image_width, &gui.crosshair.image_height);
	IM_ASSERT(ret);

	// Create the shapes seen in the 3D scene
	// ********************************************** //


	player = character(camera_control, {2, 0, 1});

	cube1 = cube(vec3{0,0,1.0f}, {0, 1, 0});

	for (int i = -5 ; i < 5; i++){
		for (int j = -5 ; j < 5; j++){
			terrain.push_back(cube({i, j, 0}));
		}
	}

	for (int i = -5 ; i < 5; i++){
		for (int j = -5 ; j < 5; j++){
			terrain.push_back(cube({i, j, 5}, {0,0,1}));
		}
	}

	for (int i = -1 ; i < 1; i++){
		terrain.push_back(cube({i, 3, 1}));
	}

	terrain.push_back(cube1);

}


// This function is called permanently at every new frame
// Note that you should avoid having costly computation and large allocation defined there. This function is mostly used to call the draw() functions on pre-existing data.
void scene_structure::display_frame()
{

	// Set the light to the current position of the camera
	environment.light = {10, 10, 10};

	// Update time
	timer.update();

	// conditional display of the global frame (set via the GUI)
	if (gui.display_frame)
		draw(global_frame, environment);

	// Draw all the shapes

	for (auto c : terrain) c.draw(environment);

}

void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);
	ImGui::Begin("Weapon", NULL ,  ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration);
	gui.portal_gun.image_height = std::min(window.width/4, 300);
	gui.portal_gun.image_width = std::min(window.width/4, 300);
	ImGui::SetWindowPos({window.width-gui.portal_gun.image_width, window.height-gui.portal_gun.image_height});
	ImGui::Image((void*)(intptr_t) gui.portal_gun.image_texture, ImVec2(gui.portal_gun.image_width, gui.portal_gun.image_height));
	ImGui::End();

	ImGui::Begin("Crosshair", NULL ,  ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration);
	gui.crosshair.image_height = std::min(50, window.width/30);
	gui.crosshair.image_width = std::min(50, window.width/30);
	ImGui::SetWindowPos({(window.width-gui.crosshair.image_width)/2, (window.height-gui.crosshair.image_height)/2});
	ImGui::Image((void*)(intptr_t) gui.crosshair.image_texture, ImVec2(gui.crosshair.image_width, gui.crosshair.image_height));
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
	player.move(terrain);
}
