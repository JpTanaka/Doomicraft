#include "cgp/cgp.hpp" // Give access to the complete CGP library
#include "environment.hpp" // The general scene environment + project variable
#include <iostream> 



// Custom scene of this code
#include "scene.hpp"

#include "audio_controller.hpp"



// *************************** //
// Custom Scene defined in "scene.hpp"
// *************************** //

scene_structure scene;


// The rest of this code is a generic initialization and animation loop that can be applied to different scenes
// *************************** //
// Start of the program
// *************************** //

window_structure standard_window_initialization(int width = 0, int height = 0);
void initialize_default_shaders();
void animation_loop();
bool main_menu();

timer_fps fps_record;

int main(int, char* argv[])
{
	std::cout << "Run " << argv[0] << std::endl;

	

	// ************************ //
	//     INITIALISATION
	// ************************ //

	// Standard Initialization of an OpenGL ready window
	scene.window = standard_window_initialization();
	scene.window.is_full_screen = true;
	scene.window.set_full_screen();

	// Initialize System Info
	project::path = cgp::project_path_find(argv[0], "shaders/");

	// Initialize default shaders
	initialize_default_shaders();

	// Custom scene initialization
	std::cout << "Initialize data of the scene ..." << std::endl;
	scene.initialize();
	std::cout << "Initialization finished\n" << std::endl;


	// ************************ //
	//     Menu Loop
	// ************************ //
	std::cout << "Start menu loop ..." << std::endl;
	while (!glfwWindowShouldClose(scene.window.glfw_window) && !main_menu()) {}


	// ************************ //
	//     Animation Loop
	// ************************ //
	std::cout << "Start animation loop ..." << std::endl;
	scene.initialize_game();
	fps_record.start();

	// sound thread	
#ifdef __linux__
	init_sound_effects();
#endif //_WIN32

	// Call the main display loop in the function animation_loop
	//  The following part is simply a loop that call the function "animation_loop"
	//  (This call is different when we compile in standard mode with GLFW, than when we compile with emscripten to output the result in a webpage.)
#ifndef __EMSCRIPTEN__
	// Default mode to run the animation/display loop with GLFW in C++
	while (!glfwWindowShouldClose(scene.window.glfw_window) && !scene.close_game) {
		animation_loop();
	}
#else
	// Specific loop if compiled for EMScripten
	emscripten_set_main_loop(animation_loop, 0, 1);
#endif

	std::cout << "\nAnimation loop stopped" << std::endl;

	// Cleanup
	cgp::imgui_cleanup();
	glfwDestroyWindow(scene.window.glfw_window);
	glfwTerminate();


}

bool main_menu(){
	bool ret = false;

	emscripten_update_window_size(scene.window.width, scene.window.height); // update window size in case of use of emscripten (not used by default)
    glfwSetInputMode(scene.window.glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	scene.camera_projection.aspect_ratio = scene.window.aspect_ratio();
	scene.environment.camera_projection = scene.camera_projection.matrix();
	glViewport(0, 0, scene.window.width, scene.window.height);

	vec3 const& background_color = scene.environment.background_color;
	glClearColor(background_color.x, background_color.y, background_color.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	float const time_interval = fps_record.update();
	glfwSetWindowTitle(scene.window.glfw_window, "DoomiCraft");

	imgui_create_frame();
	ImGui::GetIO().FontGlobalScale = project::gui_scale;
	scene.inputs.mouse.on_gui = ImGui::GetIO().WantCaptureMouse;
	scene.inputs.time_interval = time_interval;

	ImVec2 window_size(500, 600);
	ImGui::Begin("DoomiCraft", NULL, 
		ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground
	);
	ImGui::Text("		 --------------------------");
	ImGui::Text("		 | Wellcome to DoomiCraft |");
	ImGui::Text("		 --------------------------");
	ImGui::Text(" ");
	ImGui::Text("To move: A W S D");
	ImGui::Text("To choose a block: ");
	ImGui::Text("	Q for the previous");
	ImGui::Text("	E for the next");
	ImGui::Text("	1 for the Pickaxe");
	ImGui::Text(" ");
	ImGui::Text("To shoot: Right mouse button");
	ImGui::Text("To put a block/break it: Left mouse button");
	ImGui::Text(" ");
	ImGui::Text("To access the menu: Esc");
	ImGui::Text(" ");
	ImGui::Text(" ");
	ImGui::Text("Good luck and have fun!");
	ImGui::Text(" ");
	ImGui::Text(" ");


	if (ImGui::Button("Creative", {window_size.x/2.0f - 5, 50})) {
		scene.game_mode = game_modes::kCreative;
		ret = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Survival", {window_size.x/2.0f - 5, 50})) {
		scene.game_mode = game_modes::kSurvival;
		ret = true;
	}
	if (ImGui::Button("Exit", {window_size.x, 50})) {
		exit(0);
	}

	ImGui::SetWindowSize(window_size);
	ImGui::SetWindowPos({
		(float)(scene.window.width - window_size.x)/2.0f,
		(float)(scene.window.height - window_size.y)/2.0f,
	});
	ImGui::End();


	// End of ImGui display and handle GLFW events
	imgui_render_frame(scene.window.glfw_window);
	glfwSwapBuffers(scene.window.glfw_window);
	glfwPollEvents();

	return ret;
}

void animation_loop()
{

	emscripten_update_window_size(scene.window.width, scene.window.height); // update window size in case of use of emscripten (not used by default)

	scene.camera_projection.aspect_ratio = scene.window.aspect_ratio();
	scene.environment.camera_projection = scene.camera_projection.matrix();
	glViewport(0, 0, scene.window.width, scene.window.height);

	vec3 const& background_color = scene.environment.background_color;
	glClearColor(background_color.x, background_color.y, background_color.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	float const time_interval = fps_record.update();
	glfwSetWindowTitle(scene.window.glfw_window, "Doomicraft");

	imgui_create_frame();
	ImGui::GetIO().FontGlobalScale = project::gui_scale;
	scene.inputs.mouse.on_gui = ImGui::GetIO().WantCaptureMouse;
	scene.inputs.time_interval = time_interval;
	scene.fps = fps_record.fps;


	// Display the ImGUI interface (button, sliders, etc)
	scene.display_gui();

	if (!scene.game_over){
		// Handle camera behavior in standard frame
		scene.idle_frame();

		// Call the display of the scene
		scene.display_frame();
	}


	// End of ImGui display and handle GLFW events
	imgui_render_frame(scene.window.glfw_window);
	glfwSwapBuffers(scene.window.glfw_window);
	glfwPollEvents();
}


void initialize_default_shaders()
{
	// Generate the default directory from which the shaders are found
	//  By default, it should be "shaders/"
	std::string default_path_shaders = project::path +"shaders/";

	// Set standard mesh shader for mesh_drawable
	mesh_drawable::default_shader.load(default_path_shaders +"mesh/mesh.vert.glsl", default_path_shaders +"mesh/mesh.frag.glsl");
	triangles_drawable::default_shader.load(default_path_shaders +"mesh/mesh.vert.glsl", default_path_shaders +"mesh/mesh.frag.glsl");

	// Set default white texture
	image_structure const white_image = image_structure{ 1,1,image_color_type::rgba,{255,255,255,255} };
	mesh_drawable::default_texture.initialize_texture_2d_on_gpu(white_image);
	triangles_drawable::default_texture.initialize_texture_2d_on_gpu(white_image);

	// Set standard uniform color for curve/segment_drawable
	curve_drawable::default_shader.load(default_path_shaders +"single_color/single_color.vert.glsl", default_path_shaders+"single_color/single_color.frag.glsl");
}



/**
 * Set callback functions that define the gameplay.
 * 
 * Other modules might be called for better modularity of the program.
*/


//Callback functions
void window_size_callback(GLFWwindow* window, int width, int height);
void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_click_callback(GLFWwindow* window, int button, int action, int mods);
void keyboard_callback(GLFWwindow* window, int key, int, int action, int mods);

// Standard initialization procedure
window_structure standard_window_initialization(int width_target, int height_target)
{
	// Create the window using GLFW
	// ***************************************************** //
	window_structure window;
	window.initialize(width_target, height_target, "CGP Display", CGP_OPENGL_VERSION_MAJOR, CGP_OPENGL_VERSION_MINOR);

	// Display information
	// ***************************************************** //

	// Display window size
	std::cout << "\nWindow (" << window.width << "px x " << window.height << "px) created" << std::endl;
	std::cout << "Monitor: " << glfwGetMonitorName(window.monitor) << " - Resolution (" << window.screen_resolution_width << "x" << window.screen_resolution_height << ")\n" << std::endl;

	// Display debug information on command line
	std::cout << "OpenGL Information:" << std::endl;
	std::cout << cgp::opengl_info_display() << std::endl;

	// Initialize ImGUI
	// ***************************************************** //
	cgp::imgui_init(window.glfw_window);

	// Set the callback functions for the inputs
	glfwSetMouseButtonCallback(window.glfw_window, mouse_click_callback); // Event called when a button of the mouse is clicked/released
	glfwSetCursorPosCallback(window.glfw_window, mouse_move_callback);    // Event called when the mouse is moved
	glfwSetWindowSizeCallback(window.glfw_window, window_size_callback);  // Event called when the window is rescaled        
	glfwSetKeyCallback(window.glfw_window, keyboard_callback);            // Event called when a keyboard touch is pressed/released
	glfwSetScrollCallback(window.glfw_window, mouse_scroll_callback);     // Event called when scrolling the mouse

	return window;
}




// This function is called everytime the window is resized
void window_size_callback(GLFWwindow*, int width, int height)
{
	scene.window.width = width;
	scene.window.height = height;
}

// This function is called everytime the mouse is moved
void mouse_move_callback(GLFWwindow* /*window*/, double xpos, double ypos)
{
	vec2 const pos_relative = scene.window.convert_pixel_to_relative_coordinates({ xpos, ypos });
	scene.inputs.mouse.position.update(pos_relative);
	scene.mouse_move_event();
}

// This function is called everytime a mouse button is clicked/released
void mouse_click_callback(GLFWwindow* window, int button, int action, int mods)
{
	ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
	
	scene.inputs.mouse.click.update_from_glfw_click(button, action);
	scene.mouse_click_event();
}

// This function is called everytime the mouse is scrolled
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);

	scene.inputs.mouse.scroll = yoffset;
	scene.mouse_scroll_event();
}

// This function is called everytime a keyboard touch is pressed/released
void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
	bool imgui_capture_keyboard = ImGui::GetIO().WantCaptureKeyboard;
	
	if(!imgui_capture_keyboard){
		scene.inputs.keyboard.update_from_glfw_key(key, action);
		scene.keyboard_event();

		// Press 'F' for full screen mode
		if (key == GLFW_KEY_F && action == GLFW_PRESS && scene.inputs.keyboard.shift) {
			scene.window.is_full_screen = !scene.window.is_full_screen;
			if (scene.window.is_full_screen)
				scene.window.set_full_screen();
			else
				scene.window.set_windowed_screen();
		}

	}

}


