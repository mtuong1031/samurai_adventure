#pragma

#include "ECS.h"
#include "Component.h"
#include "../Game.h"
#include "../Vector2D.h"

class TheEnemies : public Component
{
    public:
        TheEnemies(int rng, int spd, Vector2D vel)
            : range(rng), speed(spd), velocity(vel)
        {   }

        ~TheEnemies()
        {   }

        void init() override
        {
            transform = &entity->getComponent<TransformComponent>();
            sprite = &entity->getComponent<SpriteComponent>();
            collider = &entity->getComponent<ColliderComponent>();
            
            transform->velocity = velocity;
        }

        void update() override
        {

            // Move to the player
            if (Game::playerRect.x > transform->position.x - range
                && Game::playerRect.x < transform->position.x + range
                && Game::playerRect.y > transform->position.y - range
                && Game::playerRect.y < transform->position.y + range) 
            {
                float length = sqrt(pow(Game::playerRect.x - transform->position.x, 2) + pow(Game::playerRect.y - transform->position.y, 2));
                float dx = (Game::playerRect.x - transform->position.x) / length;
                float dy = (Game::playerRect.y - transform->position.y) / length;

                transform->velocity.x = dx;
                transform->velocity.y = dy;
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
                    attacking = true;
                    health -= 1;
                    transform->velocity.x = -dx;
                    transform->velocity.y = -dy;
                    sprite->Play("Idle");
                } else {
                    attacking = false;
                }

            } else {
                transform->velocity.x = 0;
                transform->velocity.y = 0;
                sprite->Play("Idle");
            }

            // 
            if (health <= 0)
            {
                isDead = true;
            }
            if (isDead)
            {
                entity->destroy();
            }

        }
    private:
        TransformComponent *transform;
        SpriteComponent *sprite;
        ColliderComponent *collider;

        Vector2D velocity;

        int range = 0;
        int speed = 1;
        int attackRange = 50;
        bool attacking = false;
        bool hit = false;
        bool isDead = false;
        int health = 100;


};