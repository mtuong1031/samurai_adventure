#ifndef _SPRITECOMPONENT_H_
#define _SPRITECOMPONENT_H_

#include "Component.h"
#include "TransformComponent.h"
#include "../TextureManager.h"
#include "Animation.h"
#include <map>


/// quản lý và hiển thị các thành phần hình ảnh
class SpriteComponent : public Component {
    private:
        TransformComponent *transform;
        SDL_Texture *texture;
        SDL_Rect srcRect, destRect;

        bool animated = false;
        int frames = 0;
        int speed = 100; // tốc độ chuyển động của hình ảnh

    public:

        int anieIndex = 0;

        std::map<const char*, Animation> animations;

        // set hướng
        SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

        SpriteComponent() = default;
        SpriteComponent(const char *path) 
        {
            setTex(path);
        }

        SpriteComponent(const char *path, bool isAnimated) 
        {
            animated = true;

            Animation idle = Animation(0, 8, 100);
            Animation run = Animation(1, 8, 100);

            animations.emplace("Idle", idle);
            animations.emplace("Run", run);

            Play("Idle");
            Play("Run");
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

            if (animated) {
                srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
            }

            srcRect.y = anieIndex * transform->height;

            destRect.x = static_cast<int>(transform->position.x);  
            destRect.y = static_cast<int>(transform->position.y);  
            destRect.w = transform->width * transform->scale;
            destRect.h = transform->height * transform->scale;
        }

        void draw() override {
            TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
        }

        void Play(const char *animName) {
            frames = animations[animName].frames;
            anieIndex = animations[animName].index;
            speed = animations[animName].speed;
        }
};

#endif // _SPRITECOMPONENT_H_