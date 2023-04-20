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
    struct Triplet {
        int x, y, z;
        Triplet(int x, int y, int z) : x(x), y(y), z(z) {}
        Triplet(vec3 p) : x(p.x), y(p.y), z(p.z) {}
        bool operator==(const Triplet& other) const {
            return x == other.x && y == other.y && z == other.z;
        }
        Triplet operator+(const Triplet& other) const {
            return Triplet(x + other.x, y + other.y, z + other.z);
        }
        friend std::ostream& operator<<(std::ostream& os, const Triplet& triplet) {
            os << "(" << triplet.x << ", " << triplet.y << ", " << triplet.z << ")";
            return os;
        }
    };
    
    struct TripletHash {
        std::size_t operator()(const Triplet& triplet) const {
            std::hash<int> int_hash;
            std::size_t x_hash = int_hash(triplet.x);
            std::size_t y_hash = int_hash(triplet.y) << 1; // Shift left by 1 bit to create unique combinations
            std::size_t z_hash = int_hash(triplet.z) << 2; // Shift left by 2 bits to create unique combinations
            return x_hash ^ y_hash ^ z_hash;
        }
    };
    
} // namespace utils
