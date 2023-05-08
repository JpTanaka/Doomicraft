#include "scene.hpp"
using namespace cgp;
#include "audio_controller.hpp"


void scene_structure::initialize_game(){
	main_player.start_game();
	camera_control.activate();

	switch (game_mode){
	case game_modes::kCreative:
		environment.background_color = {0.5, 0.7, 1};
		gui.fog_depth = 24;
		lists.background_creative = true;
		break;

	case game_modes::kSurvival:
		environment.background_color = 0.05*vec3{1, 1, 1};
		gui.fog_depth = 10;
		enemies = mob_group({0, 0, 20});
		lists.background = true;
		break;
	
	case game_modes::kTest:
		environment.background_color = vec3{0, 0, 0};
		gui.fog_depth = 16;
		gui.creative = false;
		gui.collision_box = true;
		gui.debug = true;
		break;
	
	default:
		break;
	}

}

void scene_structure::initialize()
{
	camera_control.initialize(inputs, window);
	camera_control.deactivate();
	environment.uniform_generic.uniform_vec3["fog_color"] = environment.background_color;
	camera_projection.field_of_view = FIELD_OF_VIEW;
	environment.light = {50, 50, 50};
	glfwSetCursorPos(window.glfw_window, 0, 0);
	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	block::initialize();
	terr = terrain();
	main_player = player(camera_control, { 0, 0, 10 }, &gui.creative, &terr, game_mode == game_modes::kTest ? &environment : nullptr);

	// Adding portal gun
	glfwInit();
	// utils::LoadTextureFromFile("../assets/portal_gun.png", &gui.portal_gun.image_texture, &gui.portal_gun.image_width, &gui.portal_gun.image_height);
	utils::LoadTextureFromFile("../assets/crosshair_green.png", &gui.crosshair.image_texture, &gui.crosshair.image_width, &gui.crosshair.image_height);
	utils::LoadTextureFromFile("../assets/hit_crosshair.png", &gui.hit_crosshair.image_texture, &gui.hit_crosshair.image_width, &gui.hit_crosshair.image_height);

	initialize_timed_guis();
}

void scene_structure::display_frame()
{
	if (game_over) return;
	environment.light = main_player.position + vec3{0,0,10};
	environment.uniform_generic.uniform_int["fog_depth"] = gui.fog_depth;
	timer.update();
	terr.draw(environment, gui.display_wireframe, main_player.get_eyes(), main_player.looking_at(), gui.fog_depth);
	if(gui.collision_box) main_player.draw_collision_box(environment);

	switch (game_mode){
	case game_modes::kSurvival:
		enemies.draw(environment, gui.display_wireframe);
		break;

	case game_modes::kCreative:
		break;
	
	default:
		break;
	}
}

void scene_structure::end_game(){
	main_player.end_game();
	game_over = true;
	camera_control.deactivate();
	environment.background_color = 0.05 * vec3{1, 1, 1};
}

void scene_structure::mouse_move_event()
{
	if (!inputs.keyboard.shift)
		camera_control.action_mouse_move(environment.camera_view);
}

void scene_structure::mouse_click_event()
{
	if(
		main_player.handle_mouse_input(terr.get_cubes(main_player.position), enemies)
	) hit_crosshair.init(10);
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

	switch (game_mode){
	case game_modes::kSurvival:
		enemies.set_level(main_player.get_level());
		camera_control.idle_frame(environment.camera_view);
		main_player.move(terr.get_cubes(main_player.position));
		enemies.move(terr, main_player.body.position, inputs.time_interval);
		if(!gui.creative && enemies.check_hits_player(main_player.position)){
			main_player.take_hit(); 
			dead_notice.init(100);
		}
		if(!gui.creative && main_player.is_dead())
			end_game();
		break;

	case game_modes::kCreative:
		camera_control.idle_frame(environment.camera_view);
		main_player.move(terr.get_cubes(main_player.position));
		break;

	case game_modes::kTest:
		camera_control.idle_frame(environment.camera_view);
		main_player.move(terr.get_cubes(main_player.position));
		break;
	
	default:
		break;
	}

}

void scene_structure::initialize_timed_guis(){

	// hit crosshair
	render_func render = [&](){
		ImGui::Begin("Hit Crosshair", NULL, 
			ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration
		);
		gui.hit_crosshair.image_height = std::min(15.0f, window.width / 60.0f);
		gui.hit_crosshair.image_width = std::min(15.0f, window.width / 60.0f);
		ImGui::SetWindowPos({((float)window.width - (float)gui.hit_crosshair.image_width) / 2.0f,
		((float)window.height - (float)gui.hit_crosshair.image_height) / 2.0f});
		ImGui::Image(
			(void *)(intptr_t)gui.hit_crosshair.image_texture, 
			ImVec2((float)gui.hit_crosshair.image_width, 
			(float)gui.hit_crosshair.image_height)
		);
		ImGui::End();
	};
	hit_crosshair = timed_gui(render);

	render_func render_dead = [&](){
		ImVec2 size = {200, 300};
		ImGui::Begin("You died", NULL, 
			ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration
		);
		ImGui::SetWindowSize(size);
		ImGui::SetWindowPos({
			((float)window.width - size.x) / 2.0f,
			((float)window.height - size.y) / 2.0f
		});
		ImGui::Text("---->YOU DIED<----");
		ImGui::Text(" ");
		ImGui::Text(" ");
		ImGui::Text("%4d lifes left", main_player.get_health());
		ImGui::Text(" ");
		ImGui::End();
	};
	dead_notice = timed_gui(render_dead);

}

void scene_structure::display_gui()
{

	if (game_over){
		ImGui::Begin("", NULL, 
			ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground
		);
		ImGui::Text("		------------------------------");
		ImGui::Text("		--------->Game Over<----------");
		ImGui::Text("		------------------------------");
		ImGui::Text(" ");
		if (game_mode == game_modes::kSurvival){
			ImGui::Text("Better luck next time...");
			ImGui::Text(" ");
			ImGui::Text(" ");
			ImGui::Text("Total kills: %d", main_player.get_kills());
			ImGui::Text("Level reached: %d", main_player.get_level());
		}
		ImGui::Text(" ");
		ImGui::SetWindowSize(gui.end_window_size);
		ImGui::SetWindowPos({
			(float)(window.width - gui.end_window_size.x)/2.0f,
			(float)(window.height - gui.end_window_size.y)/2.0f,
		});
		if(ImGui::Button("Exit", {gui.end_window_size.x, 40})) close_game = true;
		ImGui::End();

		return;
	}


	// timed guis
	hit_crosshair.draw();
	dead_notice.draw();

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
		ImGui::Checkbox("Debug mode", &gui.debug);
		if (gui.debug){
			ImGui::Checkbox("Wireframe", &gui.display_wireframe);
			ImGui::Checkbox("Creative", &gui.creative);
			ImGui::Checkbox("Colision Box", &gui.collision_box);
			ImGui::SliderInt("Fog Depth", &gui.fog_depth, 0, 64);
		}
		ImGui::Text(" ");
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
		ImGui::Text("FPS: %d", fps);
	}
	if (game_mode == game_modes::kSurvival){
		std::stringstream s;
		for (int i = 0; i < main_player.get_health(); i++) s << "*";
		ImGui::Text("----------------------");
		ImGui::Text("| Kills:  %10d |", main_player.get_kills());
		ImGui::Text("| Level:  %10d |", main_player.get_level() + 1);
		ImGui::Text("| Health: %10s |", s.str().c_str());
		ImGui::Text("----------------------");
	}
	ImGui::End();
	

	// blocks
	ImGui::Begin("blocks", NULL, 
		ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground
	);
	ImGui::SetWindowSize(gui.block_window_size);
	ImGui::SetWindowPos({0, window.height - gui.block_window_size.y});

	ImGui::Text("----------------------------------------------------");
	ImGui::Text("| %-10s <--q--[ %-10s ]--e--> %10s |", 
		main_player.get_block(-1).c_str(),
		main_player.get_block().c_str(),
		main_player.get_block(1).c_str()
	);
	ImGui::Text("----------------------------------------------------");
	ImGui::End();

	
	// Weapon
	// ImGui::Begin("Weapon", NULL, 
	// 	ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration
	// );
	// gui.portal_gun.image_height = std::min(window.width / 4.0f, 300.0f);
	// gui.portal_gun.image_width = std::min(window.width / 4.0f, 300.0f);
	// ImGui::SetWindowPos({
	// 	(float)window.width - (float)gui.portal_gun.image_width, 
	// 	(float)window.height - (float)gui.portal_gun.image_height
	// });
	// ImGui::SetWindowSize({
	// 	(float)gui.portal_gun.image_width, (float)gui.portal_gun.image_height
	// });
	// ImGui::Image(
	// 	(void *)(intptr_t)gui.portal_gun.image_texture, 
	// 	ImVec2(gui.portal_gun.image_width, gui.portal_gun.image_height)
	// );
	// ImGui::End();

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
