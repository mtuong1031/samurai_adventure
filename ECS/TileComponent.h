#ifndef _TilelComponent_h_
#define _TilelComponent_h_

#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"    
#include "SDL2/SDL.h"

class TileComponent : public Component{
    public:

        SDL_Texture *texture;
        SDL_Rect srcRect, destRect;

        TileComponent() = default;

        ~TileComponent() {
            SDL_DestroyTexture(texture);
        }

        TileComponent(int srcX, int srcY, int xpos, int ypos, const char* path) 
        {
            texture = TextureManager::LoadTexture(path);

            srcRect.x = srcX;
            srcRect.y = srcY;
            srcRect.w = srcRect.h = 32;

            destRect.x = xpos;
            destRect.y = ypos;
            destRect.w = destRect.h = 32;
        }

        void draw() override
        {
            TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
        }

};

#endif // _TilelComponent_h_