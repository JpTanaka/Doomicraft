#pragma once
#include "cgp/cgp.hpp"

namespace utils
{
    using namespace cgp;

    // coordinate by coordinate functions
    vec3 max(vec3, vec3);
    float max(vec3);
    float min(vec3);
    vec3 min(vec3, vec3);
    vec3 relu(vec3);
    vec3 abs(vec3);
    vec3 round(vec3);

    vec2 crop(vec3);
    vec3 expand(vec2);
    vec3 project_plane(vec3);

    float distance(vec2, vec2);
    float distance(vec3, vec3);

    vec3 standardize_direction(vec3);

    int sign(float);
    float rand(float min = 0, float max = 1);

    float gaussian(vec2 r, vec2 mean, vec2 sigma);

    float distance_point_to_square(
        vec3 point_from, // point from where the ray is being shot
        vec3 direction, // direction of the ray
        vec3 plane_center, // center of the plane
        vec3 plane_normal, // plane normal
        float plane_side // the lenght of the side of the plane
    );

    float distance_point_cube(
        vec3 point_from, // point from where the ray is being shot
        vec3 direction, // direction of the ray
        vec3 cube_center, // center of the plane
        float cube_side // the lenght of the side of the plane
    );

    struct Triplet{
        int x, y, z;
        Triplet(int x, int y, int z);
        Triplet(vec3 p);
        bool operator==(const Triplet& other) const;
        Triplet operator+(const Triplet& other) const;
        friend std::ostream& operator<<(std::ostream& os, const Triplet& triplet);
    };

    struct TripletHash {
        std::size_t operator()(const Triplet& triplet) const;
    };

    bool LoadTextureFromFile(const char *filename, GLuint *out_texture, int *out_width, int *out_height);

} // namespace utils
