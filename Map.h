
#ifndef MAP_H_
#define MAP_H_

#include "Game.h"
#include "TextureManager.h"
#include <fstream>
#include <string>

class Map {
    public:
        Map();
        ~Map();
        static void LoadMap(std::string path, int sizeX, int sizeY);

    private:
        
};  

#endif