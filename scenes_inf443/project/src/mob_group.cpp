#include"mob_group.hpp"
#include"mob.hpp"
#include<vector>

mob_group::mob_group(){
}
// TODO creating mobs in mob_group results in seg fault
void mob_group::add_mob(const mob &b) {
    mobs.push_back(b);
}

std::vector<cube> mob_group::get_cubes(){
    std::vector<cube> cubes;
    for(mob& m : mobs){
        cubes.push_back(m.body);
        cubes.push_back(m.legs);
    }
    return cubes;
}

void mob_group::shoot_mob(
    vec3 point_from, // point from where the ray is being shot
    vec3 direction, // direction of the ray
    float distance // distance of the closest cube
) {
    direction = normalize(direction);
    vec3 final_point = point_from + (distance + kEps) * direction;

    // shooting
    for (auto it = mobs.begin(); it != mobs.end(); it++) {
        if (
            utils::distance(final_point, it->body.position) < Length  ||
            utils::distance(final_point, it->legs.position) < Length
        ){
            it->life--;
        } 
    }

    // killing

}

int mob_group::check_dead(){
    int kills = 0;
    std::vector<std::vector<mob>::iterator> deads;
    for (auto it = mobs.begin(); it != mobs.end(); it++) 
        if(it->life == 0)
            deads.push_back(it);
    
    for (auto& dead: deads) {
        mobs.erase(dead);
        kills ++;
    }
    return kills;
}

void mob_group::move(terrain& terr, vec3& position_player, const float& dt) {
    for(mob& m : mobs)
        m.move(terr.get_cubes(m.position), position_player, dt);
}

void mob_group::draw(const environment_structure& env,bool wireframe){
    for(mob& m : mobs) {
        m.draw(env, wireframe);
    }
}

