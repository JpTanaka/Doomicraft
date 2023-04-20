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

    float distance(vec2, vec2);

    vec2 standardize_direction(vec3);

    int sign(float);
    float rand();

    float gaussian(vec2 r, vec2 mean, vec2 sigma);

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

} // namespace utils
