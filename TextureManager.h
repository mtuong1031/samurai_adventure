#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "CommonFunc.h"

class TextureManager {
    public:
        TextureManager();
        ~TextureManager();
        
        virtual bool LoadTexture(const char *fileName, SDL_Renderer *ren);
        void RenderTexture(SDL_Renderer* tex, const SDL_Rect* clip);
        void free();

    private:
        SDL_Texture* texture;
        SDL_Rect rect;
};


#endif // TEXTURE_MANAGER_H