#pragma once

#include "ECS.h"
#include "Component.h"
#include "../Game.h"

class KeyboardControler : public Component
{
    public:
        Uint32 attack_cd = 0;
        Uint32 block_cd = 0;
        Uint32 skill_cd = 0;
        Vector2D BulletVel;
        SDL_RendererFlip flip = SDL_FLIP_NONE;

        int health = 100;
        int max_health = 100;
        bool isDead = false;
        bool isAttacking = false;
        bool isSkill = false;
        bool attack_up = false;
        bool attack_down = false;
        bool hit = false;
        bool isBlocking = false;

        void init() override {
            transform = &entity->getComponent<TransformComponent>();
            sprite = &entity->getComponent<SpriteComponent>();
            BulletVel = {0, 0};
        }

        void update() override 
        {

            if (isAnimating) {
                Uint32 currentTick = SDL_GetTicks();

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
                    sprite->Play("Run_up");
                    BulletVel = {0, - 1};
                    attack_up = true;
                    Mix_PlayChannel(-1, Game::runSound, 0);
                    break;
                case SDLK_a:
                    transform->velocity.x = -1;
                    sprite->Play("Run");
                    sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                    flip = SDL_FLIP_HORIZONTAL;
                    BulletVel = {-1, 0};
                    Mix_PlayChannel(-1, Game::runSound, 0);
                    break;
                case SDLK_s:
                    transform->velocity.y = 1;
                    sprite->Play("Run");
                    BulletVel = {0, 1};
                    attack_down = true;
                    Mix_PlayChannel(-1, Game::runSound, 0);
                    break;
                case SDLK_d:
                    transform->velocity.x = 1;
                    sprite->Play("Run");
                    sprite->spriteFlip = SDL_FLIP_NONE;
                    flip = SDL_FLIP_NONE;
                    BulletVel = {1, 0};
                    Mix_PlayChannel(-1, Game::runSound, 0);
                    break;
                case SDLK_j:
                    // transform->velocity.x = 0;
                    // transform->velocity.y = 0;
                    if (attack_up) {
                        sprite->Play("Idle_up");
                    } else {
                        sprite->Play("Attack_x");
                    }
                    isAttacking = true;
                    attack_cd = SDL_GetTicks();
                    break;
                case SDLK_i:
                    sprite->Play("Attack_x");
                    isSkill = true;
                    skill_cd = SDL_GetTicks();
                    break;
                case SDLK_k:
                    transform->velocity.x = 0;
                    transform->velocity.y = 0;
                    sprite->Play("Idle");
                    isBlocking = true;
                    block_cd = SDL_GetTicks();
                    break;
                case SDLK_LSHIFT:
                    sprite->Play("Rool");
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
                    BulletVel = {0, - 1};
                    attack_up = true;
                    sprite->Play("Idle_up");
                    break;
                case SDLK_a:
                    transform->velocity.x = 0;
                    BulletVel = {-1, 0};
                    sprite->Play("Idle");  
                    sprite->spriteFlip = SDL_FLIP_HORIZONTAL; 
                    flip = SDL_FLIP_HORIZONTAL;
                    break;
                case SDLK_s:
                    transform->velocity.y = 0;
                    BulletVel = {0, 1};
                    attack_down = true;
                    sprite->Play("Idle");
                    break;
                case SDLK_d:
                    transform->velocity.x = 0;
                    BulletVel = {1, 0};
                    sprite->Play("Idle");
                    sprite->spriteFlip = SDL_FLIP_NONE;
                    flip = SDL_FLIP_NONE;
                    break;
                case SDLK_j:
                    sprite->Play("Idle");
                    isAttacking = false;
                    break;
                case SDLK_i:
                    sprite->Play("Idle");
                    isSkill = false;
                    break;
                case SDLK_k:
                    sprite->Play("Idle");
                    isBlocking = false;
                    break;
                case SDLK_LSHIFT:
                    sprite->Play("Idle");
                    break;
                default:
                    break;
                }
            }

            if (hit) {
                sprite->Play("Hit");
                hit = false;
            } 
        }
    
    private:
        const char* currentAnimationName;
        TransformComponent *transform;
        SpriteComponent *sprite;
        Vector2D velocity;

        int currentFrame = 0;
        int height;
        int width;
        bool isAnimating = false;
        bool isShooting = false;

};

