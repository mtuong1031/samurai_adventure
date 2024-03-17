#include <Map.h>
#include <TextureManager.h>
#include <CommonFunc.h>
Map::Map() {
    // Khởi tạo các thành phần của map
    sand = TextureManager::LoadTexture("assets/sand.png");
    tree = TextureManager::LoadTexture("assets/tree.png");
    water = TextureManager::LoadTexture("assets/water.png");
    rock = TextureManager::LoadTexture("assets/rock.png");
}

Map::~Map() {
    SDL_DestroyTexture(sand);
    SDL_DestroyTexture(tree);
    SDL_DestroyTexture(water);
    SDL_DestroyTexture(rock);
}

void Map::LoadMap(const char *name) {
    std::ifstream file(name);
    if (!file) {
        std::cout << "Khong the mo file" << std::endl;
        return;
    }
    for (int i = 0; i < 35; i++) {
        for (int j = 0; j < 40; j++) {
            file >> map[i][j];
        }
    }
    file.close();
}

void Map::DrawMap(SDL_Renderer *ren) {
    int type = 0;
    for (int i = 0; i < 35; i++) {
        for (int j = 0; j < 40; j++) {
            type = map[i][j];
            dest.x = j * 64;
            dest.y = i * 64;
            dest.w = 64;
            dest.h = 64;
            switch (type) {
                case 0:
                    TextureManager::RenderTexture(sand, ren, src, dest);
                    break;
                case 1:
                    TextureManager::RenderTexture(tree, ren, src, dest);
                    break;
                case 2:
                    TextureManager::RenderTexture(water, ren, src, dest);
                    break;
                case 3:
                    TextureManager::RenderTexture(rock, ren, src, dest);
                    break;
                default:
                    break;
            }
        }
    }
}