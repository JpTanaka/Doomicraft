#include "scene.hpp"

using namespace cgp;

void scene_structure::initialize()
{
	camera_projection.field_of_view = FIELD_OF_VIEW;
	camera_control.initialize(inputs, window);
	environment.light = {50, 50, 50};
	environment.uniform_generic.uniform_vec3["fog_color"] = environment.background_color;
	glfwSetCursorPos(window.glfw_window, 0, 0);

	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	block::initialize();
	terr = terrain();
	main_player = player(camera_control, {0, 0, 10}, &gui.creative, &terr);

	enemies = mob_group({0, 0, 20});

	// Adding portal gun
	glfwInit();
	utils::LoadTextureFromFile("../assets/portal_gun.png", &gui.portal_gun.image_texture, &gui.portal_gun.image_width, &gui.portal_gun.image_height);
	utils::LoadTextureFromFile("../assets/crosshair.png", &gui.crosshair.image_texture, &gui.crosshair.image_width, &gui.crosshair.image_height);
}

void scene_structure::display_frame()
{
	if (game_over) return;
	environment.uniform_generic.uniform_int["fog_depth"] = gui.fog_depth;
	timer.update();
	enemies.draw(environment, gui.display_wireframe);
	terr.draw(environment, gui.display_wireframe, main_player.get_eyes(), main_player.looking_at(), gui.fog_depth);
}

void scene_structure::end_game(){
	game_over = true;
	camera_control.deactivate();
}

void scene_structure::mouse_move_event()
{
	if (!inputs.keyboard.shift)
		camera_control.action_mouse_move(environment.camera_view);
}

void scene_structure::mouse_click_event()
{
	main_player.handle_mouse_input(terr.get_cubes(main_player.position), enemies);
	camera_control.action_mouse_click(environment.camera_view);
}

void scene_structure::keyboard_event()
{
	main_player.handle_keyboard_input();
	camera_control.action_keyboard(environment.camera_view, gui.display_config);
}

void scene_structure::idle_frame()
{
	if (gui.display_config) return;

	enemies.set_level(main_player.get_level());
	camera_control.idle_frame(environment.camera_view);
	main_player.move(terr.get_cubes(main_player.position));

	// TODO
	enemies.move(terr, main_player.body.position, main_player.camera->inputs->time_interval);
	

	if(!gui.creative && enemies.check_hits_player(main_player.position)){
		main_player.take_hit(); // respawns the player
	}

	if(!gui.creative && main_player.is_dead())
		end_game();
}

void scene_structure::display_gui()
{

	if (game_over){
		ImGui::Begin("", NULL, 
			ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground
		);
		ImGui::Text("---------------------");
		ImGui::Text("------Game Over------");
		ImGui::Text("---------------------");
		ImGui::Text(" ");
		ImGui::Text("Total kills: %d", main_player.get_kills());
		ImGui::Text("---------------------");
		ImGui::Text(" ");
		ImGui::SetWindowSize(gui.config_window_size);
		ImGui::SetWindowPos({
			(float)(window.width - gui.config_window_size.x)/2.0f,
			(float)(window.height - gui.config_window_size.y)/2.0f,
		});
		if(ImGui::Button("Exit")) {
			close_game = true;
		}
		ImGui::End();

		return;
	}
	// Menu
	if (gui.display_config){
		ImGui::Begin("Configuration ", NULL, 
			ImGuiWindowFlags_NoDecoration
		);
		ImGui::Text("Configurations");
		ImGui::SetWindowSize(gui.config_window_size);
		ImGui::SetWindowPos({
			(float)(window.width - gui.config_window_size.x)/2.0f,
			(float)(window.height - gui.config_window_size.y)/2.0f,
		});
		ImGui::Checkbox("Wireframe", &gui.display_wireframe);
		ImGui::Checkbox("Creative", &gui.creative);
		ImGui::Checkbox("Debug mode", &gui.debug);
		ImGui::SliderInt("Fog Depth", &gui.fog_depth, 0, 64);
		if(ImGui::Button("Exit", {gui.config_window_size.x, 40})) end_game();
		ImGui::End();
	}

	// UI
	ImGui::Begin("stats", NULL, 
		ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground
	);
	ImGui::SetWindowSize(gui.stats_window_size);
	ImGui::SetWindowPos({0, 0});

	if (gui.debug){
		ImGui::Text("Position: (%.2f, %.2f, %.2f)", 
			main_player.position.x,
			main_player.position.y, 
			main_player.position.z
		);
		ImGui::Text("Looking at: (%.2f, %.2f, %.2f)", 
			main_player.looking_at().x, 
			main_player.looking_at().y, 
			main_player.looking_at().z
		);
		auto t = chunk::get_chunk_triplet(main_player.position);
		ImGui::Text("Chunk: (%d, %d, %d)", 
			t.x, t.y, t.z
		);
	}
	ImGui::Text("-----------------");
	ImGui::Text("Kills: %d", main_player.get_kills());
	std::stringstream s;
	for (int i = 0; i < main_player.get_health(); i++) s << "*";
	ImGui::Text("Health: %s", s.str().c_str());
	ImGui::Text("-----------------");
	ImGui::End();
	
	// Weapon
	ImGui::Begin("Weapon", NULL, 
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration
	);
	gui.portal_gun.image_height = std::min(window.width / 4.0f, 300.0f);
	gui.portal_gun.image_width = std::min(window.width / 4.0f, 300.0f);
	ImGui::SetWindowPos({
		(float)window.width - (float)gui.portal_gun.image_width, 
		(float)window.height - (float)gui.portal_gun.image_height
	});
	ImGui::SetWindowSize({
		(float)gui.portal_gun.image_width, (float)gui.portal_gun.image_height
	});
	ImGui::Image(
		(void *)(intptr_t)gui.portal_gun.image_texture, 
		ImVec2(gui.portal_gun.image_width, gui.portal_gun.image_height)
	);
	ImGui::End();

	// Crosshair
	ImGui::Begin("Crosshair", NULL, 
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration
	);
	gui.crosshair.image_height = std::min(30.0f, window.width / 30.0f);
	gui.crosshair.image_width = std::min(30.0f, window.width / 30.0f);
	ImGui::SetWindowPos({
		((float)window.width - (float)gui.crosshair.image_width) / 2.0f,
		((float)window.height - (float)gui.crosshair.image_height) / 2.0f
	});
	ImGui::Image(
		(void *)(intptr_t)gui.crosshair.image_texture, 
		ImVec2((float)gui.crosshair.image_width, (float)gui.crosshair.image_height)
	);
	ImGui::End();
}
