#include"mob_group.hpp"
#include"mob.hpp"
#include<vector>

mob_group::mob_group(){}

mob_group::mob_group(vec3 starting_position){
    for (int i = 0; i < wave_size; i++){
        vec3 rand_vec = {utils::rand(-1, 1) * mob_range, utils::rand(-1, 1) * mob_range, 7};
        add_mob(mob({
            starting_position + rand_vec
        }));
    }
}

// TODO creating mobs in mob_group results in seg fault
void mob_group::add_mob(const mob b) {
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

bool mob_group::shoot_mob(
    vec3 point_from, // point from where the ray is being shot
    vec3 direction, // direction of the ray
    float distance // distance of the closest cube
) {
    direction = normalize(direction);
    vec3 final_point = point_from + (distance + kEps) * direction;
    bool hit = false;

    // shooting
    for (auto it = mobs.begin(); it != mobs.end(); it++) {
        if (
            utils::distance(final_point, it->body.position) < Length  ||
            utils::distance(final_point, it->legs.position) < Length
        ){
            it->take_damage();
            hit = true;
        } 
    }
    return hit;
}

int mob_group::check_dead(vec3 from){
    int kills = 0;
    std::vector<std::vector<mob>::iterator> deads;
    for (auto it = mobs.begin(); it != mobs.end(); it++) 
        if(it->is_dead())
            deads.push_back(it);
    
    for (auto& dead: deads) {
        // creating another one around the dead
        vec3 rand_vec;
        do {
            rand_vec = {utils::rand(-1, 1) * mob_range, utils::rand(-1, 1) * mob_range, 10};
        } while (norm(from - rand_vec) < 10);
        
        add_mob(mob({
            from + rand_vec
        }));

        // erasing the dead
        mobs.erase(dead);
        kills++;
    }
    return kills;
}

void mob_group::move(terrain& terr, vec3& position_player, const float& dt) {
    for(mob& m : mobs)
        m.move(terr.get_cubes(m.position), position_player, dt, velocity);
}

void mob_group::draw(const environment_structure& env,bool wireframe){
    for(mob& m : mobs) {
        m.draw(env, wireframe);
    }
}


bool mob_group::check_hits_player(vec3 player_position){
    for (mob& m : mobs) 
        if (norm(m.position - player_position) < Length) return true;
    return false;
}

void mob_group::set_level(int level){
    wave_size = 5 + level;
    mob_range = 25 - std::min(level, 15);
    velocity = 1 + std::min(level, 3);
}
