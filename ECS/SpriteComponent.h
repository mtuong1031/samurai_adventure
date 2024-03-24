#ifndef _SPRITECOMPONENT_H_
#define _SPRITECOMPONENT_H_

#include "Component.h"
#include "PositionComponents.h"


/// quản lý và hiển thị các thành phần hình ảnh
class SpriteComponent : public Component {
    private:
        PositionComponent *position;
        SDL_Texture *texture;
        SDL_Rect srcRect, destRect;

    public:
        SpriteComponent() = default;
        SpriteComponent(const char *path) {
            setTex(path);
        }

        void setTex(const char *path) {
            texture = TextureManager::LoadTexture(path);
        }

        void init() override {
            position = &entity->getComponent<PositionComponent>();  // lấy vị trí của entity

            srcRect.x = srcRect.y = 0;
            srcRect.w = srcRect.h = 32; 
            destRect.w = destRect.h = 64;  
        }

        void update() override {  
            destRect.x = position->x();  
            destRect.y = position->y();
        }

        void draw() override {
            TextureManager::Draw(texture, srcRect, destRect);
        }
};

#endif // _SPRITECOMPONENT_H_