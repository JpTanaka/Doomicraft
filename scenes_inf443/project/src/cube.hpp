#pragma once

#include "cgp/cgp.hpp"
#include "environment.hpp"

// This definitions allow to use the structures: mesh, mesh_drawable, etc. without mentionning explicitly cgp::
using cgp::mesh;
using cgp::mesh_drawable;
using cgp::numarray;
using cgp::timer_basic;
using cgp::vec3;


class cube {
    mesh_drawable mesh;

public:
    float L = 1.0f;
    vec3 position;
    cube();
    cube(vec3 center, vec3 color = {1, 0, 0});
    void draw(environment_structure&);
    vec3 colision(cube const&);
    bool check_colision(cube const&, double tolerance = 0.0);
    float distance(cube const&);
};
