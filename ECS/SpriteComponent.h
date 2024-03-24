#ifndef _SPRITECOMPONENT_H_
#define _SPRITECOMPONENT_H_

#include "Component.h"
#include "TransformComponent.h"
#include "../TextureManager.h"


/// quản lý và hiển thị các thành phần hình ảnh
class SpriteComponent : public Component {
    private:
        TransformComponent *transform;
        SDL_Texture *texture;
        SDL_Rect srcRect, destRect;

    public:
        SpriteComponent() = default;
        SpriteComponent(const char *path) {
            setTex(path);
        }

        ~SpriteComponent() {
            SDL_DestroyTexture(texture);
        }

        void setTex(const char *path) {
            texture = TextureManager::LoadTexture(path);
        }

        void init() override 
        {
            transform = &entity->getComponent<TransformComponent>();  // lấy vị trí của entity

            srcRect.x = srcRect.y = 0;
            srcRect.w = transform->width;
            srcRect.h = transform->height; 
        }

        void update() override {  
            destRect.x = (int)transform->position.x;  
            destRect.y = (int)transform->position.y;
            destRect.w = transform->width * transform->scale;
            destRect.h = transform->height * transform->scale;
        }

        void draw() override {
            TextureManager::Draw(texture, srcRect, destRect);
        }
};

#endif // _SPRITECOMPONENT_H_