
#ifndef MAP_H_
#define MAP_H_

#include "CommonFunc.h"
#include "TextureManager.h"

class Map 
{
    public:
        Map();
        ~Map();
        
        void LoadMap(const char* name);
        void DrawMap(SDL_Renderer* ren);

    private:
        SDL_Rect src, dest;

        //Các thành phần của map
        SDL_Texture*  sand;
        SDL_Texture* tree;
        SDL_Texture* water;
        SDL_Texture* rock;

        int map[20][30];
}


#endif  