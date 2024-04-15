#pragma once

#include "SDL2/SDL.h"
#include <map>

#include "Component.h"
#include "../TextureManager.h"
#include "Animation.h"
#include "../AssetManager.h"
#include "TransformComponent.h"

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
        const char* currentAnimationName;
        Uint32 aniStartTime = 0;
        SDL_Rect playersRect;
        bool Attack_lr = false;

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
            animated = isAnimated;

            // Animation idle = Animation(0, 4, 200, 64, 64);
            // Animation run = Animation(1, 6, 100, 48, 64);
            // Animation attack_x = Animation(2, 8, 50, 112, 80);
            // Animation hit = Animation(3, 4, 150, 64, 64);
            // Animation attack_dow = Animation(4, 8, 50, 112, 112);
            // Animation attack_up = Animation(5, 8, 50, 128, 112);
            Animation idle = Animation(0, 5, 100, 64, 80);
            Animation run = Animation(1, 8, 100, 64, 80);
            Animation attack_x = Animation(2, 8, 100, 196, 112);
            Animation attack_gun = Animation(3, 8, 100, 91, 80);
            Animation hit = Animation(4, 5, 150, 64, 80);

            animations.emplace("Idle", idle);   
            animations.emplace("Run", run);
            animations.emplace("AttackX", attack_x);
            animations.emplace("AttackGun", attack_gun);
            animations.emplace("Hit", hit);

            // animations.emplace("Hit", hit);
            // animations.emplace("AttackDow", attack_dow);
            // animations.emplace("AttackUp", attack_up);

            Play("Idle");          

            setTex(id);
        }

        SpriteComponent(std::string id, bool isAnimated, int numFrames, int speed, 
                        int width, int height, int rattck_width, int attack_height) 
        {
            animated = isAnimated;

            Animation idle = Animation(0, numFrames, speed, width, height);
            Animation run = Animation(1, numFrames, speed, width, height);
            Animation attack = Animation(2, numFrames, 150, rattck_width, attack_height);
            Animation hit = Animation(3, numFrames, 150, width, height);

            animations.emplace("Idle", idle);   
            animations.emplace("Run", run);
            animations.emplace("Attack", attack);
            animations.emplace("Hit", hit);

            Play("Idle");

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
                srcRect.x = srcRect.w * static_cast<int>(((SDL_GetTicks()) / speed) % frames);
            }
            srcRect.y = anieIndex * transform->height;
            srcRect.w = animations[currentAnimationName].width;
            srcRect.h = animations[currentAnimationName].height;

            playersRect.h = animations[currentAnimationName].height;
            playersRect.w = animations[currentAnimationName].width;

            if (spriteFlip == SDL_FLIP_NONE) {
                destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
            } else {
                destRect.x = static_cast<int>(transform->position.x) - Game::camera.x - animations[currentAnimationName].width + transform->width * transform->scale;
            }
            destRect.y = static_cast<int>(transform->position.y) - Game::camera.y - animations[currentAnimationName].height + transform->height * transform->scale; 
            destRect.w = animations[currentAnimationName].width * transform->scale;
            destRect.h = animations[currentAnimationName].height * transform->scale;

            playersRect.x = destRect.x;
            playersRect.y = destRect.y;
        }

        void draw() override {
            TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
        }

        void Play(const char *animName) 
        {
            currentAnimationName = animName;

            frames = animations[animName].frames;
            anieIndex = animations[animName].index;
            speed = animations[animName].speed;

            aniStartTime = SDL_GetTicks();  
        }
};
