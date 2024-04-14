#pragma once

#include "../Game.h"
#include "Component.h"

class Enemy : public Component
{
    public:
        TransformComponent *transform;
        SpriteComponent *sprite;
        KeyboardControler *player;
        SDL_Rect collider;
        Vector2D velocity;
        std::map<const char*, Animation> animations;

        int range_attack = 30;
        int range = 50;
        int speed = 3;

        Enemy(int r, int s) : range(r), speed(s) 
        {   }

        ~Enemy() 
        {   }

        void init() override
        {
            transform = &entity->getComponent<TransformComponent>();
            sprite = &entity->getComponent<SpriteComponent>();
            collider = {0, 0, transform->width, transform->height};
        }

        void update() override
        {
            collider.x = static_cast<int>(transform->position.x);
            collider.y = static_cast<int>(transform->position.y);

            // if (player->getRectX("player") < transform->position.x + range 
            //     && player->getRectX("player") > transform->position.x - range 
            //     && player->getRectY("player") < transform->position.y + range 
            //     && player->getRectY("player") > transform->position.y - range)
            // {
            //     Vector2D temp;
            //     temp.x = player->getRectX("player");
            //     temp.y = player->getRectY("player");

            //     velocity = temp - transform->position;
            //     // velocity = velocity.Normalize();

            //     sprite->Play("Run");
            //     transform->velocity = velocity * speed;
            // }
            // else if (player->getRectX("player") < transform->position.x + range_attack 
            //         && player->getRectX("player") > transform->position.x - range_attack 
            //         && player->getRectY("player") < transform->position.y + range_attack 
            //         && player->getRectY("player") > transform->position.y - range_attack)
            // {
            //     sprite->Play("Attack");
            //     transform->velocity.Zero();
            // }
            // else
            // {
            //     sprite->Play("Idle");
            //     transform->velocity.Zero();
            // }
            
        }
};

// if (Game::player.x < transform->position.x + range && Game::player.x > transform->position.x - range &&
//                 Game::player.y < transform->position.y + range && Game::player.y > transform->position.y - range)
//             {
//                 velocity = Game::player - transform->position;
//                 velocity = velocity.normalize();
//                 sprite->Play("Run");
//                 transform->velocity = velocity * speed;
//             }
//             else if (Game::player.x < transform->position.x + range_attack && Game::player.x > transform->position.x - range_attack &&
//                      Game::player.y < transform->position.y + range_attack && Game::player.y > transform->position.y - range_attack)
//             {
//                 sprite->Play("Attack");
//                 transform->velocity.Zero();
//             }
//             else
//             {
//                 sprite->Play("Idle");
//                 transform->velocity.Zero();
//             }
