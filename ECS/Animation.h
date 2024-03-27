#ifndef _Animation_H_
#define _Animation_H_

struct Animation {
    int index;
    int frames;
    int speed;

    Animation() {}
    Animation(int i, int f, int s) {
        index = i;
        frames = f;
        speed = s;
    }
    
};

#endif // _Animation_H_