#pragma once

#include "Game.h"
#include "TextureManager.h"


class GameObject {
    private:
        int xpos;
        int ypos;

        SDL_Texture* objTexture;
        SDL_Rect srcRect, destRect;

    public:
        GameObject(const char* texturesheet, SDL_Renderer* ren, int x, int y);
        ~GameObject() {};

        void Update();
        void Render();
};