#pragma once
#include "cgp/cgp.hpp"

namespace utils
{
    using namespace cgp;

    // coordinate by coordinate functions
    vec3 max(vec3, vec3);
    vec3 min(vec3, vec3);
    vec3 relu(vec3);
    vec3 abs(vec3);

    vec2 crop(vec3);
    vec3 expand(vec2);
    vec3 project_plane(vec3);

    vec2 standardize_direction(vec3);

    int sign(float);
    
} // namespace utils
