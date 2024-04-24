#pragma once

#include "ECS.h"
#include "Component.h"
#include "../Game.h"
#include "../Vector2D.h"
#include "../Collision.h"

class TheBosses : public Component
{
    public:
        TransformComponent *transform;
        SpriteComponent *sprite;
        ColliderComponent *collider;
        ProjectileComponent *projectile;

        Uint32 lastick = 0;
        const char* currentAnimation;

        int health = 1000;
        int max_health = 1000;
        int damage = 10;
        bool hit = false;
        bool isAttacking;
        bool inRange = false;
        int status = 0;

        TheBosses(Vector2D OriVel, int rng, int spd, Vector2D vel)
            :  original_vector(OriVel), range(rng), speed(spd), velocity(vel)
        {   }

        TheBosses(Vector2D OriVel, int rng, int spd, Vector2D vel, int hp)
            :  original_vector(OriVel), range(rng), speed(spd), velocity(vel), health(hp)
        {   }

        ~TheBosses()
        {   }

        void init() override
        {
            transform = &entity->getComponent<TransformComponent>();
            sprite = &entity->getComponent<SpriteComponent>();
            collider = &entity->getComponent<ColliderComponent>();
            projectile = &entity->getComponent<ProjectileComponent>();
            
            transform->velocity = velocity;
            isAttacking = false;
        }

        Vector2D getDirection(Vector2D target)
        {
            float length = sqrt(pow(target.x - transform->position.x, 2) + pow(target.y - transform->position.y, 2));
            float dx = (target.x - transform->position.x) / length;
            float dy = (target.y - transform->position.y) / length;
            return {dx, dy};
        }

        void update() override
        {

            if (health > 750) 
                status = 0;
            else if (health > 400 && health< 750)
                status = 1;
            else if (health > 0 && health < 400)
                status = 2;

            float length = sqrt(pow(Game::playerRect.x - transform->position.x, 2) + pow(Game::playerRect.y - transform->position.y, 2));
            float dx = (Game::playerRect.x - transform->position.x) / length;
            float dy = (Game::playerRect.y - transform->position.y) / length;

            // Move to the player
            if (Game::playerRect.x > transform->position.x - range
                && Game::playerRect.x < transform->position.x + range
                && Game::playerRect.y > transform->position.y - range
                && Game::playerRect.y < transform->position.y + range) 
            {
                isAttacking = true;
                inRange = true;
                transform->velocity.x = dx * 0.1;
                transform->velocity.y = dy * 0.1;
                if (dx > 0) {
                    sprite->spriteFlip = SDL_FLIP_NONE;
                } else {
                    sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                }
                sprite->Play("Run");

                if (Game::playerRect.x > transform->position.x - attackRange
                    && Game::playerRect.x < transform->position.x + attackRange
                    && Game::playerRect.y > transform->position.y - attackRange
                    && Game::playerRect.y < transform->position.y + attackRange) 
                {
                    if (dx > 0) {
                        sprite->spriteFlip = SDL_FLIP_NONE;
                    } else {
                        sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                    }
                        transform->velocity.x = 0;
                        transform->velocity.y = 0;
                        sprite->Play("Idle");  
                        lastick = SDL_GetTicks();

                    if (hit) {
                        sprite->Play("Hit");
                        // transform->position.x -= transform->velocity.x * 300;
                        // transform->position.y -= transform->velocity.y * 300;
                        std::cout << "Hit  " << std::endl;
                        hit = false;
                    }
                }

                if (hit) {
                    sprite->Play("Hit");
                    // transform->position.x -= transform->velocity.x * 300;
                    // transform->position.y -= transform->velocity.y * 300;
                    std::cout << "Hit  " << std::endl;
                    hit = false;
                }

            } else {
                transform->velocity.x = 0;
                transform->velocity.y = 0;
                sprite->Play("Idle");
                inRange = false;
                isAttacking = false;
            }

            if (health <= 0)
                isDead = true;
            
            if (isDead)
                entity->destroy();
        }

    private:
        Vector2D velocity;
        SDL_Rect colliderRect;

        Vector2D original_vector;
        int range = 0;
        int speed = 1;
        int attackRange = 1;
        bool isDead = false;
};