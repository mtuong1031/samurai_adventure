#ifndef _TilelComponent_h_
#define _TilelComponent_h_

#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"    
#include "SDL2/SDL.h"

class TileComponent : public Component{
    public:
        TransformComponent *transform;
        SpriteComponent *sprite;

        SDL_Rect tileRect;
        int tileID; 
        const char* path;

        TileComponent() = default;

        TileComponent(int x, int y, int w, int h, int id){
            tileRect.x = x;
            tileRect.y = y;
            tileRect.w = w;
            tileRect.h = h;
            tileID = id;

            switch (tileID)
            {
                case 0:
                    break;
                case 1:
                    path = "assets/base7.png";
                    break;
                case 2:
                    path = "assets/base8.png";
                    break;
                case 3:
                    path = "assets/base9.png";
                    break;
                default:
                    break;
            }
        }

        void init() override {
            entity->addComponent<TransformComponent>((float)tileRect.x, (float)tileRect.y, tileRect.w, tileRect.h, 1);    
            transform = &entity->getComponent<TransformComponent>();

            entity->addComponent<SpriteComponent>(path);
            sprite = &entity->getComponent<SpriteComponent>();
        }
};

#endif // _TilelComponent_h_