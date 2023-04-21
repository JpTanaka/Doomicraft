#include"mob_group.hpp"
#include"mob.hpp"
#include<vector>

mob_group::mob_group(){
}
// TODO creating mobs in mob_group results in seg fault
void mob_group::add_mob(const mob &b) {
    mobs.push_back(b);
}

const std::vector<mob>& mob_group::get_mobs(){
    return mobs;
}

std::vector<cube> mob_group::get_cubes(){
    std::vector<cube> cubes;
    for(mob& m : mobs){
        cubes.push_back(m.body);
        cubes.push_back(m.legs);
    }
    return cubes;
}

void mob_group::erase_mob(auto it) {
    mobs.erase(it);
}

void mob_group::shoot_mob(
    vec3 point_from, // point from where the ray is being shot
    vec3 direction, // direction of the ray
    float distance // distance of the closest cube
) {
    float eps = 0.001;
    vec3 final_point = point_from + distance*direction + eps;
    for(auto it = mobs.begin(); it!=mobs.end(); it++) {
        if(utils::distance(final_point, it->body.position)<Length/2 || utils::distance(final_point, it->legs.position)<Length/2){
            std::cout << "tomou tiro " << it->body.position << std::endl;
            mobs.erase(it);
            break;
        } 
    }
}

void mob_group::move(terrain &terr, vec3 &position_player, const float &dt) {
    for(mob& m : mobs) {
        m.move(terr.get_cubes(m.position), position_player, dt);
    }
}

void mob_group::draw(const environment_structure& env,bool wireframe){
    for(mob& m : mobs) {
        m.draw(env, wireframe);
    }
}

