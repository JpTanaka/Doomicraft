#pragma once
#include "cgp/cgp.hpp"
#include "environment.hpp"

enum billboard_types {
    red_flower,
    red_tulip,
    paeonia,
    blue_flower,
    NUM_BILLBOARDS
};

extern std::array<std::pair<mesh_drawable, mesh_drawable>, billboard_types::NUM_BILLBOARDS> billboards;

class billboard{
    vec3 position;
    billboard_types type;

public:
    billboard(billboard_types, vec3);
    billboard();
    void draw(const environment_structure&, bool wireframe) const;

    vec3 get_position() const;
    void update_position(vec3);

    static void init_billboards();
};