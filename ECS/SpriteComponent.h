#pragma once

#include "SDL2/SDL.h"
#include <map>

#include "Component.h"
#include "../TextureManager.h"
#include "Animation.h"
#include "../AssetManager.h"
#include  "TransformComponent.h"


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
        SpriteComponent(std::string id) 
        {
            setTex(id);
        }

        SpriteComponent(std::string id, bool isAnimated) 
        {
            animated = true;

            Animation idle = Animation(0, 5, 100, 64, 80);
            Animation run = Animation(1, 8, 100, 64, 80);
            Animation attack = Animation(2, 10, 100, 80, 144);    

            animations.emplace("Idle", idle);   
            animations.emplace("Run", run);
            animations.emplace("Attack", attack);

            Play("Idle");
            Play("Run");
            Play("Attack");

            setTex(id);
        }

        ~SpriteComponent() {
        }

        void setTex(std::string id) {
            texture = Game::assets->GetTexture(id);
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

            destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;  
            destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;  
            destRect.w = transform->width * transform->scale;
            destRect.h = transform->height * transform->scale;
        }

        void draw() override {
            TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
        }

        void Play(const char *animName) 
        {
            frames = animations[animName].frames;
            anieIndex = animations[animName].index;
            speed = animations[animName].speed;
        }
};
