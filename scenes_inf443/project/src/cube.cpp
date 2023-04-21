#include "cube.hpp"
#include "utils.hpp"
#include "constants.hpp"
#include <initializer_list>
#include <algorithm>

cube::cube(vec3 center){
    position = center;
};
cube::cube(){};

vec3 cube::colision(cube const& c){
    float dx = c.position.x - position.x;
    float dy = c.position.y - position.y;
    float dz = c.position.z - position.z;
    
    float adx = std::abs(dx);
    float ady = std::abs(dy);
    float adz = std::abs(dz);


    if( !check_colision(c) ) return {0, 0, 0};

    if (adx > ady && adx > adz) return {dx, 0, 0};
    if (ady > adx && ady > adz) return {0, dy, 0};
    if (adz > ady && adz > adx) return {0, 0, dz};

    return {0, 0, 0};
}

bool cube::check_colision(cube const& c, double tolerance){
    return std::abs(distance(c) - Length ) < tolerance;
}

std::pair<int, int> cube::get_colision_direction(cube const& c, double tolerance){
    /**
     * returns a pair:
     *  first: the coordinate the colision is taking place (0 for x, 1 for y , 2 for z, -1 if no colision)
     *  second: positive or negative semiaxis (1 for positive, -1 for negative, 0 for no collision)
     * the direction is from this perspective (inverse for cube "c" perspective)
    */
    if (!check_colision(c, tolerance)) return {-1, 0}; // if they are not coliding

    float distance = this->distance(c);

    vec3 dr = c.position - position;
    for(int i = 0; i < 3; i++)
        if (std::abs(std::abs(dr[i])  - distance) < tolerance) {
            vec3 dir = {0,0,0};
            dir[i] = 1;
            return {i, dot(dr, dir) < 0  ? -1 : 1};
        }

    return {-1, 0};
}

float cube::distance(cube const& c){
    float dx = std::abs(c.position.x - position.x);
    float dy = std::abs(c.position.y - position.y);
    float dz = std::abs(c.position.z - position.z);
    return std::max({dx, dy, dz});
}

float cube::distancexy(cube const& c){
    float dx = std::abs(c.position.x - position.x);
    float dy = std::abs(c.position.y - position.y);
    return std::max(dx, dy);
}



float cube::top(){
    return position.z + Length/2.0f;
}

float cube::bottom(){
    return position.z - Length/2.0f;
}

bool cube::operator==(const cube& other) const {
    return (
        other.position.x == position.x &&
        other.position.y == position.y &&
        other.position.z == position.z
    );
}