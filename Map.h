
#ifndef MAP_H_
#define MAP_H_

#include "Game.h"
#include "TextureManager.h"

class Map {
    public:
        Map();
        ~Map();
        void LoadMap(const char *path);
        void DrawMap();

    private:
        SDL_Rect src, dest;
        SDL_Texture* base;
        int map[20][25];
};  

#endif