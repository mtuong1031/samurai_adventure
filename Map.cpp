#include "Map.h"
#include "TextureManager.h"
#include "CommonFunc.h"
Map::Map() {
    renderer = NULL;
    texture = NULL;
}

Map::~Map() {
}

bool Map::LoadObject(const char *fileName, SDL_Renderer *ren) {
    SDL_Texture* new_texture = NULL; // Bien tam de load hinh anh

    SDL_Surface* tmpsurface = IMG_Load(fileName); 

    if (tmpsurface != NULL) 
    {
        new_texture = SDL_CreateTextureFromSurface(ren, tmpsurface); 
        SDL_FreeSurface(tmpsurface); 
    } else {
        std::cout << "Can't load image" << std::endl;
    }

    texture = new_texture;
    return texture != NULL;
}

void Map::getMapValue() {
    Map::LoadObject("assets/base1.png", renderer);
    Map::LoadObject("assets/base2.png", renderer);
    Map::LoadObject("assets/base3.png", renderer);
    Map::LoadObject("assets/base4.png", renderer);
    Map::LoadObject("assets/base5.png", renderer);
    Map::LoadObject("assets/base6.png", renderer);
    Map::LoadObject("assets/base7.png", renderer);
    Map::LoadObject("assets/base8.png", renderer);
    Map::LoadObject("assets/base9.png", renderer);
}

void Map::LoadMap(const char *name) {
    std::ifstream file(name);
    if (!file.is_open()) {
        std::cout << "Can't open file" << std::endl;
        return;
    }
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 60; j++) {
            file >> map[i][j];
        }
    }
    file.close();
}


void Map::DrawMap(SDL_Renderer *ren) {
    int type = 0;
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 60; j++) {
            type = map[i][j];
            dest.x = j *32;
            dest.y = i * 32;
            dest.w = 32;
            dest.h = 32;
            switch (type) {
                case 1:
                    LoadObject("assets/base1.png", ren);
                    SDL_RenderCopy(ren, texture, NULL, &dest);
                    break;
                case 2:
                    Map::LoadObject("assets/base2.png", ren);
                    SDL_RenderCopy(ren, texture, NULL, &dest);
                    break;
                case 3:
                    Map::LoadObject("assets/base3.png", ren);
                    SDL_RenderCopy(ren, texture, NULL, &dest);
                    break;
                case 4:
                    Map::LoadObject("assets/base4.png", ren);
                    SDL_RenderCopy(ren, texture, NULL, &dest);
                    break;
                case 5:
                    Map::LoadObject("assets/base5.png", ren);
                    SDL_RenderCopy(ren, texture, NULL, &dest);
                    break;
                case 6:
                    Map::LoadObject("assets/base6.png", ren);
                    SDL_RenderCopy(ren, texture, NULL, &dest);
                    break;
                case 7:
                    Map::LoadObject("assets/base7.png", ren);
                    SDL_RenderCopy(ren, texture, NULL, &dest);
                    break;
                case 8:
                    Map::LoadObject("assets/base8.png", ren);
                    SDL_RenderCopy(ren, texture, NULL, &dest);
                    break;
                case 9:
                    Map::LoadObject("assets/base9.png", ren);
                    SDL_RenderCopy(ren, texture, NULL, &dest);
                    break;
                default:
                    break;
            }
            
        }
    }
}