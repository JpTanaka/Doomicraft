#pragma once
#include "cgp/cgp.hpp"
#include "environment.hpp"
#include "cube.hpp"
#include "block.hpp"

class terrain {

    int generator_function(int x, int y);
    std::vector<block> blocks;
    std::vector<cube> cubes;

    std::vector<std::tuple<double, vec2, vec2>> gaussians;


    static const int 
        xmin = -10, 
        xmax =  10, 
        ymin = -10, 
        ymax =  10,
        zmin = -5, 
        zmax =  10;

public:
    terrain();
    ~terrain();
    void draw(environment_structure&, vec3 eyes);
    std::vector<cube> get_cubes();
};