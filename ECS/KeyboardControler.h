#ifndef _KeyboardControler_h_
#define _KeyboardControler_h_

#include "ECS.h"
#include "Component.h"
#include "../Game.h"
#include "TransformComponent.h"

class KeyboardControler : public Component
{
    public:
        TransformComponent *transform;
        SpriteComponent *sprite;

        void init() override {
            transform = &entity->getComponent<TransformComponent>();
            sprite = &entity->getComponent<SpriteComponent>();
        }
        void update() override 
        {
            if (Game::event.type == SDL_KEYDOWN) 
            {
                switch (Game::event.key.keysym.sym) {
                case SDLK_w:
                    transform->velocity.y = -1;
                    sprite->Play("Idle");
                    break;
                case SDLK_a:
                    transform->velocity.x = -1;
                    sprite->Play("Idle");
                    sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                    break;
                case SDLK_s:
                    transform->velocity.y = 1;
                    sprite->Play("Idle");
                    break;
                case SDLK_d:
                    transform->velocity.x = 1;
                    sprite->Play("Idle");
                    sprite->spriteFlip = SDL_FLIP_NONE;
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
                    sprite->Play("Run");
                    break;
                case SDLK_a:
                    transform->velocity.x = 0;
                    sprite->Play("Run");  
                    sprite->spriteFlip = SDL_FLIP_NONE; 
                    break;
                case SDLK_s:
                    transform->velocity.y = 0;
                    sprite->Play("Run");
                    break;
                case SDLK_d:
                    transform->velocity.x = 0;
                    sprite->Play("Run");
                    sprite->spriteFlip = SDL_FLIP_NONE;
                    break;
                default:
                    break;
                }
            }
        }
};


#endif // _KeyboardControler_h_