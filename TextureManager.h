#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "CommonFunc.h"
#include "Game.h"

class TextureManager {
    public:
        TextureManager();
        ~TextureManager();
        static SDL_Texture* LoadTexture(const char* fileName);
        static void Draw(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);
    private:
        SDL_Texture* texture;
        SDL_Rect rect;
};


#endif // TEXTURE_MANAGER_H