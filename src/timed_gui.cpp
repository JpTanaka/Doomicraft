#include "timed_gui.hpp"
#include <iostream> 


timed_gui::timed_gui()
    :render()
{}

timed_gui::timed_gui(render_func& render)
    :render(render)
{}

void timed_gui::init(int number_frames){
    this->number_frames = number_frames;
    this->frame_count = 0;
}

void timed_gui::draw(){
    if (frame_count > number_frames) return;
    render();
    frame_count++;
}