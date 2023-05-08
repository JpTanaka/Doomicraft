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
    mesh_drawable* mesh;

public:
    vec3 position;

    cube();
    cube(vec3 center);

    vec3 colision(cube const&);
    float distance(cube const&);
    float distancexy(cube const&);

    bool check_colision(cube const&, double tolerance = 0.1);
    std::pair<int, int> get_colision_direction(cube const&, double tolerance = 0.1);

    bool operator==(const cube&) const;

    float top() const;
    float bottom() const;


    // debugging
    void draw_wire(environment_structure&) const;
};
