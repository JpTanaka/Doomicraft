#pragma once

#include "cgp/cgp.hpp"
#include "environment.hpp"

// This definitions allow to use the structures: mesh, mesh_drawable, etc. without mentionning explicitly cgp::
using cgp::mesh;
using cgp::mesh_drawable;
using cgp::numarray;
using cgp::timer_basic;
using cgp::vec3;

struct camera_controller_custom : camera_controller_first_person_euler
{

	float mouse_sensitivity = 3.0;
	float keyboard_sensitivity = 5.0;
	float const pi = std::atan(1) * 4;

	camera_controller_custom();

	void action_mouse_move(mat4 &camera_matrix_view);

	void action_keyboard(mat4& );

	void idle_frame(mat4& camera_matrix_view);

	void set_position(vec3& position);

	private:
	    bool cursor_enabled = false;
};