#include "Map.h"
#include "ECS/ECS.h"
#include "ECS/Component.h"

extern Manager manager;

Map::Map(const char* mfp, int ms, int ts) : mapFilePath(mfp), mapScale(ms), tileSize(ts)
{
    scaledSize = ms * ts;
}

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
            srcY = atoi(&tile) * tileSize;
            mapFile.get(tile);
            srcX = atoi(&tile) * tileSize;

            AddTile(srcX, srcY, x * scaledSize, y * scaledSize);
            mapFile.ignore();
        }
    }

    mapFile.ignore();

    for(int y = 0; y < sizeY; y++)
    {
        for(int x = 0; x < sizeX; x++)
        {
            mapFile.get(tile);
            if(tile == '1')
            {
                auto &tileCollider(manager.addEntity());
                tileCollider.addComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize,  scaledSize);
                tileCollider.addGroup(Game::groupColliders);            
            }
            mapFile.ignore();
        }
    }

    mapFile.close();
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos) 
{
    auto &tile(manager.addEntity());
    tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, mapFilePath);
    tile.addGroup(Game::groupMap);
}