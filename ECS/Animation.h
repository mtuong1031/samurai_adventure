#pragma once

struct Animation {
    int index;
    int frames;
    int speed;
    int width;
    int height;

    Animation() {}
    Animation(int i, int f, int s, int w, int h) {
        index = i;
        frames = f;
        speed = s;
        width = w;
        height = h;
    }
    
};
