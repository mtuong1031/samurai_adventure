
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
        int map[20][30];
        bool LoadObject(const char *fileName, SDL_Renderer* ren);
        void LoadMap(const char* name);
        void DrawMap (SDL_Renderer* ren);
        int getMapValue(int x, int y) {
            return map[y][x]; 
        }
    private:
        SDL_Rect src, dest;

        //Các thành phần của map
        SDL_Texture*  sand;
        SDL_Texture* tree;
        SDL_Texture* water;
        SDL_Texture* rock;

        SDL_Texture* texture;
};


#endif  