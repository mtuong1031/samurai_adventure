
#ifndef MAP_H_
#define MAP_H_

#include "CommonFunc.h"
#include "TextureManager.h"
#include <fstream>

#define MAX_TILES 20

class TileMap : public TextureManager
{
    public:
        TileMap() {;}
        ~TileMap() {;}
};

class Map
{
    public:
        Map();
        ~Map();

        void LoadMap(const char* name);
        void GetMapTiles(SDL_Renderer* screen);
        int getMapValue(int x, int y);
        void DrawMap(SDL_Renderer* screen);
    private:
        Map_game mapdata;
        TileMap tile_map[MAX_TILES];
        SDL_Rect tile_rect;
};

#endif