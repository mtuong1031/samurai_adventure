#pragma once

#include "ECS.h"
#include "Component.h"
#include "../Game.h"

class KeyboardControler : public Component
{
    public:
        Uint32 lastTick;

        void init() override {
            transform = &entity->getComponent<TransformComponent>();
            sprite = &entity->getComponent<SpriteComponent>();
        }

        void update() override 
        {
            if (isAnimating) {
                Uint32 currentTick = SDL_GetTicks();

                if (currentTick - lastTick >= sprite->animations[currentAnimationName].speed) {
                    currentFrame++;
                    lastTick = currentTick;
                }

                if (currentFrame >= sprite->animations[currentAnimationName].frames) {
                    currentFrame = 0;
                    isAnimating = false;

                    const Uint8* state = SDL_GetKeyboardState(NULL);
                    if (state[SDL_SCANCODE_A]) {
                        sprite->Play("Run");
                        sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                        transform->velocity.x = -1;

                    } else if (state[SDL_SCANCODE_D]) {
                        sprite->Play("Run");
                        sprite->spriteFlip = SDL_FLIP_NONE;
                        transform->velocity.x = 1;

                    } else sprite->Play("Idle");
                }
            }



            if (Game::event.type == SDL_KEYDOWN) 
            {
                switch (Game::event.key.keysym.sym) {
                case SDLK_w:
                    transform->velocity.y = -1;
                    sprite->Play("Run");
                    break;
                case SDLK_a:
                    transform->velocity.x = -1;
                    sprite->Play("Run");
                    sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                    sprite->Attack_lr = true;
                    break;
                case SDLK_s:
                    transform->velocity.y = 1;
                    sprite->Play("Run");
                    break;
                case SDLK_d:
                    transform->velocity.x = 1;
                    sprite->Play("Run");
                    sprite->spriteFlip = SDL_FLIP_NONE;
                    sprite->Attack_lr = false;
                    break;
                case SDLK_j:
                    transform->velocity.x = 0;
                    sprite->Play("AttackGun");
                    break;
                case SDLK_ESCAPE:
                    Game::isRunning = false;
                    break;
                default:
                    break;
                }
            }
            if (Game::event.type == SDL_KEYUP) {
                switch (Game::event.key.keysym.sym) {
                case SDLK_w:
                    transform->velocity.y = 0;
                    sprite->Play("Idle");
                    break;
                case SDLK_a:
                    transform->velocity.x = 0;
                    sprite->Play("Idle");  
                    sprite->spriteFlip = SDL_FLIP_HORIZONTAL; 
                    sprite->Attack_lr = true;
                    break;
                case SDLK_s:
                    transform->velocity.y = 0;
                    sprite->Play("Idle");
                    break;
                case SDLK_d:
                    transform->velocity.x = 0;
                    sprite->Play("Idle");
                    sprite->spriteFlip = SDL_FLIP_NONE;
                    sprite->Attack_lr = false;
                    break;
                case SDLK_j:
                    transform->velocity.x = 0;
                    sprite->Play("Idle");
                    break;
                default:
                    break;
                }
            }

            if(Game::event.type == SDL_MOUSEBUTTONDOWN)
            {
                switch (Game::event.button.button)
                {
                case SDL_BUTTON_LEFT:
                    sprite->Play("AttackX");
                    currentFrame++;
                    transform->velocity.x = 0;
                    isAnimating = true;
                    lastTick = SDL_GetTicks();
                    currentAnimationName = "AttackX";
                    break;
                default:
                    break;
                }
            }
            if(Game::event.type == SDL_MOUSEBUTTONUP)
            {
                switch (Game::event.button.button)
                {
                case SDL_BUTTON_LEFT:
                    transform->velocity.x = 0;
                    sprite->Play("Idle");
                    break;
                default:
                    break;
                }
            }
        }
    
    private:
        int currentFrame = 0;
        int height;
        int width;
        bool isAnimating = false;
        bool isAttacking = false;
        bool isShooting = false;
        const char* currentAnimationName;
        TransformComponent *transform;
        SpriteComponent *sprite;
        Vector2D velocity;

};

