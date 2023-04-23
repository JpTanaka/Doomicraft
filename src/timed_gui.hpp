#pragma once
#include <functional>

using render_func = std::function<void()>;

class timed_gui {

    int number_frames = 0;
    int frame_count = 0;
    render_func render;

public:
    timed_gui();
    timed_gui(render_func& render);
    void init(int number_frames);
    void draw();
};

