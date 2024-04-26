#include <fstream>
#include "Map.h"
#include "Game.h"
#include "ECS/ECS.h"
#include "ECS/Component.h"

extern Manager manager;


Map::Map(std::string tID, int ms, int ts) : texID(tID), mapScale(ms), tileSize(ts)
{
    scaledSize = ms * ts;
}

Map::~Map() 
{}

void Map::LoadMap(std::string path, int sizeX, int sizeY) 
{
    char tile;
    std::fstream mapFile(path);

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

    mapFile.ignore();

    for (int y = 0; y < sizeY; y++) 
    {
        for (int x = 0; x < sizeX; x++)
        {
            mapFile.get(tile);
            if (tile == '2') 
            {
                Game::assets->CreateEnemies(Vector2D(x * scaledSize, y * scaledSize), Vector2D(0, 0), 400, 1, "enemy");
                std::cout << "Create enemy at " << x << " " << y << std::endl;
            }
            if (tile == '4') {
                Game::assets->CreateBoss(Vector2D(x * scaledSize, y * scaledSize), Vector2D(0, 0), 600, 1, "boss", SDL_Rect{0, 0, 125, 110});
                std::cout << "Create boss at " << x << " " << y << std::endl;
            }
            if (tile == '3') {
                Game::assets->CreateItem(Vector2D(x * scaledSize, y * scaledSize), Vector2D(0, 0), 0, 100, "chest", SDL_Rect{0, 0, 48, 48});
            }

            mapFile.ignore();
        }
    }

    mapFile.close();
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos) 
{
    auto &tile(manager.addEntity());
    tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID);
    tile.addGroup(Game::groupMap);
}