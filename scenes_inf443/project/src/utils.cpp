#include "utils.hpp" 

namespace utils
{
    using namespace cgp;

    // coordinate by coordinate functions
    vec3 max(vec3 u, vec3 v){
        return {
            std::max(u.x, v.x),
            std::max(u.y, v.y),
            std::max(u.z, v.z)
        };
    }
    vec3 min(vec3 u, vec3 v){
        return {
            std::min(u.x, v.x),
            std::min(u.y, v.y),
            std::min(u.z, v.z)
        };
    }
    vec3 relu(vec3 v){
        return max({0, 0, 0}, v);
    }
    vec3 abs(vec3 v){
        return relu(v) - relu(-v);
    }

    vec2 crop(vec3 v){
        return {v.x, v.y};
    }
    vec3 expand(vec2 v){
        return {v.x, v.y, 0};
    }
    vec3 project_plane(vec3 v){
        return mat3{{1,0,0},{0,1,0},{0,0,0}} * v;
    }
    vec2 standardize_direction(vec3 v){
        return crop(normalize(project_plane(v))); //TODO if v is too low, core dumped
    }


    int sign(float x){
        return x < 0 ? -1 : 1;
    }


    float gaussian(vec2 r, vec2 mean, vec2 sigma){
        return std::exp( - std::pow(norm((r - mean)/sigma), 2));
    }

} // namespace utils

