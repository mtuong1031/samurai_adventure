
#ifndef MAP_H_
#define MAP_H_

#include "CommonFunc.h"
#include "TextureManager.h"
#include <fstream>

class Map 
{
    public:
        Map();
        ~Map();
        int map[20][60];
        bool LoadObject(const char *fileName, SDL_Renderer* ren);
        void getMapValue();
        void LoadMap(const char* name);
        void DrawMap (SDL_Renderer* ren);
        int getMapValue(int x, int y) {
            return map[y][x]; 
        }
    private:
        SDL_Rect src, dest;

        SDL_Texture* texture;
};


#endif  