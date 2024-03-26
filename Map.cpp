#include "Map.h"


Map::Map() {
    base = TextureManager::LoadTexture("assets/base1.png");

    src.x = src.y = 0;
    src.w = dest.w = 32;
    src.h = dest.h = 32;
    dest.x = dest.y = 0;

    LoadMap("map_tiles_1.txt");
}

Map::~Map() {
    SDL_DestroyTexture(base);
}

void Map::LoadMap(const char *path) {
    std::fstream mapFile;
    mapFile.open(path);
    if (!mapFile) {
        std::cout << "Failed to open map file" << std::endl;
        return;
    }

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 25; j++) {
            mapFile >> map[i][j];
        }
    }

    mapFile.close();
}

void Map::DrawMap () {
    int type = 0;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 25; j++) {

            type = map[i][j];
            dest.x = j * 32;
            dest.y = i * 32;    

            switch (type) {
                case 0:
                    TextureManager::Draw(base, src, dest);
                    break;
                default:
                    break;
            }
        }
    }
}