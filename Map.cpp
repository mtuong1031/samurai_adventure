#include "Map.h"


Map::Map() 
{}

Map::~Map() 
{}

void Map::LoadMap(std::string path, int sizeX, int sizeY) 
{
    char tile;
    std::fstream mapFile;
    mapFile.open(path);

    int srcX, srcY;

    if (!mapFile) {
        std::cout << "Failed to open map file" << std::endl;
        return;
    }

    for (int y = 0; y < sizeY; y++)
    {
        for (int x = 0; x < sizeX; x++)
        {
            mapFile.get(tile);
            srcY = atoi(&tile) * 32;
            mapFile.get(tile);
            srcX = atoi(&tile) * 32;

            Game::AddTile(srcX, srcY, x * 32, y * 32);
            mapFile.ignore();
        }
    }

    mapFile.close();
}