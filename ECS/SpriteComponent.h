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
        SDL_Rect playersRect;

        std::map<const char*, Animation> animations;
        // set hướng
        SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

        SpriteComponent() = default;
        SpriteComponent(std::string id) 
        {
            setTex(id);
        }

        // player
        SpriteComponent(std::string id, bool isAnimated) 
        {
            animated = isAnimated;

            Animation idle = Animation(0, 2, 200, 19, 19);
            Animation run = Animation(1, 4, 100, 19, 19);
            Animation run_up = Animation(2, 4, 100, 19, 19);
            Animation rool = Animation(3, 4, 100, 19, 19);
            Animation attack_x = Animation(4, 4, 100, 19, 19);
            Animation attack_up = Animation(5, 4, 100, 19, 19);
            Animation attack_dow = Animation(6, 4, 100, 19, 19);
            Animation idle_up = Animation(7, 2, 200, 19, 19);
            Animation hit = Animation(8, 2, 100, 19, 19);

            animations.emplace("Idle", idle);
            animations.emplace("Run", run);
            animations.emplace("Run_up", run_up);
            animations.emplace("Rool", rool);
            animations.emplace("Attack_x", attack_x);
            animations.emplace("Attack_up", attack_up);
            animations.emplace("Attack_dow", attack_dow);
            animations.emplace("Idle_up", idle_up);
            animations.emplace("Hit", hit);

            Play("Idle"); 

            setTex(id);
        }

        // enemy
        SpriteComponent(std::string id, bool isAnimated, int numFrames, int speed, 
                        int width, int height, int rattck_width, int attack_height) 
        {
            animated = isAnimated;

            Animation idle = Animation(0, numFrames, speed, width, height);
            Animation run = Animation(1, numFrames, speed, width, height);
            Animation attack = Animation(2, numFrames, 200, rattck_width, attack_height);
            Animation hit = Animation(3, numFrames, 500, width, height);
            Animation skill = Animation(4, numFrames, 100, rattck_width, attack_height);

            animations.emplace("Idle", idle);   
            animations.emplace("Run", run);
            animations.emplace("Attack", attack);
            animations.emplace("Hit", hit);
            animations.emplace("Skill", skill);

            Play("Idle");

            setTex(id);
        }

        // assets
        SpriteComponent(std::string id, bool isAnimated, int numFrames, int speed, 
                        int width, int height, SDL_RendererFlip flip) 
        {
            animated = isAnimated;
            spriteFlip = flip;

            if (id == "effect") {
                Animation idle = Animation(0, numFrames, speed, 18, 30);
                Animation run = Animation(1, numFrames, speed, 35, 18);
                Animation attack = Animation(2, numFrames, speed, 35, 18);
                Animation hit = Animation(3, numFrames, speed, 11, 17);

                animations.emplace("Idle", idle);   
                animations.emplace("Run", run);
                animations.emplace("Attack", attack);
                animations.emplace("Hit", hit);

                Play("Idle");
            }

            if (id == "attack_up" || id == "attack_down") {
                Animation idle = Animation(0, numFrames, speed, 35, 18);
                Animation attack = Animation(1, numFrames, speed, 35, 18);

                animations.emplace("Idle", idle);   
                animations.emplace("Attack", attack);

                Play("Idle");
            }

            if (id == "skill") {
                Animation idle = Animation(0, 6, speed, 32, 10);
                Animation run = Animation(1, 2, speed, 410, 60);

                animations.emplace("Idle", idle);   
                animations.emplace("Run", run);

                Play("Idle");
            }

            if (id == "ssss") {
                Animation idle = Animation(0, 6, speed, 120, 90);

                animations.emplace("Idle", idle);   

                Play("Idle");
            }

            if (id == "skill2") {
                Animation idle = Animation(0, 4, speed, 15, 15);

                animations.emplace("Idle", idle);   

                Play("Idle");
            }

            if (id == "chest") {
                Animation idle = Animation(0, 6, speed, 48, 48);

                animations.emplace("Idle", idle);   

                Play("Idle");
            }

            if (id == "sb1") {
                Animation idle = Animation(0, 6, speed,48, 48);

                animations.emplace("Idle", idle);   

                Play("Idle");
            }

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

            playersRect.h = animations[currentAnimationName].height*transform->scale;
            playersRect.w = animations[currentAnimationName].width*transform->scale;

            if (spriteFlip == SDL_FLIP_HORIZONTAL) {
                destRect.x = static_cast<int>(transform->position.x)
                - animations[currentAnimationName].width*transform->scale + transform->width*transform->scale;
            } else {
                destRect.x = static_cast<int>(transform->position.x);
            }
            playersRect.x = destRect.x - transform->width*transform->scale/3;

            destRect.x -= Game::camera.x;
            destRect.y = static_cast<int>(transform->position.y) - Game::camera.y; 
            destRect.w = animations[currentAnimationName].width * transform->scale;
            destRect.h = animations[currentAnimationName].height * transform->scale;

            playersRect.y = transform->position.y;
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
        }
};
