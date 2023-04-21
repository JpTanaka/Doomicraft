#include "utils.hpp" 
#include "constants.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
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
    vec3 round(vec3 v){
        return {
            std::round(v.x),
            std::round(v.y),
            std::round(v.z)
        };
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

    float rand(float min, float max){
        return min + (max - min) * std::rand() / static_cast<float>(RAND_MAX);
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
        plane_normal = normalize(plane_normal);
        direction = normalize(direction);
        if(dot(direction, plane_normal) == 0) return INFTY;
        float t = - dot(point_from - plane_center, plane_normal) / dot(direction, plane_normal);
        vec3 interception_point = point_from + t * direction;
        if (t < 0) return INFTY;
        float dist = distance(plane_center, interception_point);
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

    // Simple helper function to load an image into a OpenGL texture with common settings
    bool LoadTextureFromFile(const char *filename, GLuint *out_texture, int *out_width, int *out_height)
    {
    	// Load from file
    	int image_width = 0;
    	int image_height = 0;
    	unsigned char *image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    	if (image_data == NULL)
    		return false;

    	// Create a OpenGL texture identifier
    	GLuint image_texture;
    	glGenTextures(1, &image_texture);
    	glBindTexture(GL_TEXTURE_2D, image_texture);

    	// Setup filtering parameters for display
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    	// Upload pixels into texture
    #if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    #endif
    	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    	stbi_image_free(image_data);

    	*out_texture = image_texture;
    	*out_width = image_width;
    	*out_height = image_height;

    	return true;
    }

} // namespace utils

