#pragma once

#include "cgp/cgp.hpp"
#include "environment.hpp"
#include "camera.hpp"
#include "player.hpp"
#include "cube.hpp"
#include "block.hpp"
#include "terrain.hpp"
#include "mob.hpp"
#include "mob_group.hpp"
#include "timed_gui.hpp"




// This definitions allow to use the structures: mesh, mesh_drawable, etc. without mentionning explicitly cgp::
using cgp::mesh;
using cgp::mesh_drawable;
using cgp::numarray;
using cgp::timer_basic;
using cgp::vec3;


// Game modes
enum game_modes {
	kSurvival,
	kCreative
};

// for loading sprites in the ImGui
struct image_obj {
	int image_width = 0;
	int image_height = 0;
	GLuint image_texture = 0;
};
// Variables associated to the GUI (buttons, etc)
struct gui_parameters
{
	bool display_config = false;

	bool display_wireframe = false;
	bool creative = false;

	bool debug = false;

	int fog_depth = 24;
	
	image_obj portal_gun;
	image_obj crosshair;
	image_obj hit_crosshair;
	
	ImVec2 config_window_size = {500, 250};

	ImVec2 stats_window_size = {400, 300};
	ImVec2 end_window_size = {500, 300};
	ImVec2 block_window_size = {600, 120};
};

// The structure of the custom scene
struct scene_structure : cgp::scene_inputs_generic
{

	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //
	camera_controller_custom camera_control;
	camera_projection_perspective camera_projection;
	window_structure window;


	mesh_drawable global_frame;		   // The standard global frame
	environment_structure environment; // Standard environment controler
	input_devices inputs;			   // Storage for inputs status (mouse, keyboard, window dimension)
	gui_parameters gui;				   // Standard GUI element storage

	int fps;


	game_modes game_mode = game_modes::kSurvival;

	// ****************************** //
	// Elements and shapes of the scene
	// ****************************** //

	timer_basic timer;

	terrain terr;
	player main_player;
	block b;
	mob_group enemies;

	bool game_over = false;
	bool close_game = false;


	timed_gui hit_crosshair;
	timed_gui dead_notice;

	// soundeffects
	bool is_shooting = false;


	// ****************************** //
	// Functions
	// ****************************** //

	void initialize_game();	 	 
	void initialize_timed_guis();
	void initialize();	  // Standard initialization to be called before the animation loop
	void display_frame(); // The frame display to be called within the animation loop
	void display_gui();	  // The display of the GUI, also called within the animation loop

	void mouse_move_event();
	void mouse_click_event();
	void keyboard_event();
	void idle_frame();
	void end_game();
};

bool LoadTextureFromFile(const char *filename, GLuint *out_texture, int *out_width, int *out_height);
