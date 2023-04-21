#include "utils.hpp" 
#include "constants.hpp"

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

    float max(vec3 v){
        return std::max({
            v.x, v.y, v.z
        });
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
        return relu(v) + relu(-v);
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

    float rand(){
        return std::rand() / static_cast<float>(RAND_MAX);
    }

    float distance(vec2 v, vec2 u){
        return std::max(
            std::abs((v-u).x),
            std::abs((v-u).y)
        );
    }

    float distance(vec3 v, vec3 u){
        return max(abs(v - u));
    }


    float gaussian(vec2 r, vec2 mean, vec2 sigma){
        return std::exp( - std::pow(norm((r - mean)/sigma), 2));
    }

    float distance_point_to_square(
        vec3 point_from, // point from where the ray is being shot
        vec3 direction, // direction of the ray
        vec3 plane_center, // center of the plane
        vec3 plane_normal, // plane normal
        float plane_side // the lenght of the side of the plane
    ){
        if(dot(direction, plane_normal) == 0) return INFTY;

        float t = dot(point_from - plane_center, plane_normal)
            / dot(direction, plane_normal);
        
        if (t < 0) return INFTY;
        float dist = distance(plane_center, point_from + t * direction);
        if(dist > plane_side / 2.0f) return INFTY;
        return t;
    }

    float distance_point_cube(
        vec3 point_from, // point from where the ray is being shot
        vec3 direction, // direction of the ray
        vec3 cube_center, // center of the plane
        float cube_side // the lenght of the side of the plane
    ){
        return std::min({
            distance_point_to_square(point_from, direction, cube_center + 0.5*cube_side*vec3{ 1, 0, 0}, { 1, 0, 0}, cube_side),
            distance_point_to_square(point_from, direction, cube_center + 0.5*cube_side*vec3{-1, 0, 0}, {-1, 0, 0}, cube_side),
            distance_point_to_square(point_from, direction, cube_center + 0.5*cube_side*vec3{ 0, 1, 0}, { 0, 1, 0}, cube_side),
            distance_point_to_square(point_from, direction, cube_center + 0.5*cube_side*vec3{ 0,-1, 0}, { 0,-1, 0}, cube_side),
            distance_point_to_square(point_from, direction, cube_center + 0.5*cube_side*vec3{ 0, 0, 1}, { 0, 0, 1}, cube_side),
            distance_point_to_square(point_from, direction, cube_center + 0.5*cube_side*vec3{ 0, 0,-1}, { 0, 0,-1}, cube_side)
        });
    }


    Triplet::Triplet(int x, int y, int z) : x(x), y(y), z(z) {}
    Triplet::Triplet(vec3 p) : x(p.x), y(p.y), z(p.z) {}
    bool Triplet::operator==(const Triplet& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    Triplet Triplet::operator+(const Triplet& other) const {
        return Triplet(x + other.x, y + other.y, z + other.z);
    }
    std::ostream& operator<<(std::ostream& os, const Triplet& triplet) {
        os << "(" << triplet.x << ", " << triplet.y << ", " << triplet.z << ")";
        return os;
    }
    
    std::size_t TripletHash::operator()(const Triplet& triplet) const {
        std::hash<int> int_hash;
        std::size_t x_hash = int_hash(triplet.x);
        std::size_t y_hash = int_hash(triplet.y) << 1; // Shift left by 1 bit to create unique combinations
        std::size_t z_hash = int_hash(triplet.z) << 2; // Shift left by 2 bits to create unique combinations
        return x_hash ^ y_hash ^ z_hash;
    }

} // namespace utils

