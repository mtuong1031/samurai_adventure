#pragma

#include <unordered_map>

#include "ECS.h"
#include "Component.h"
#include "../Game.h"
#include "../Vector2D.h"
#include "../Collision.h"

class TheEnemies : public Component
{
    public:

        Uint32 lastick;
        std::unordered_map<const char*, int> cooldowns; 
        int currentFrame = 0;
        const char* currentAnimation;

        TheEnemies(Vector2D OriVel, int rng, int spd, Vector2D vel)
            :  original_vector(OriVel), range(rng), speed(spd), velocity(vel)
        {   }

        ~TheEnemies()
        {   }

        void init() override
        {
            transform = &entity->getComponent<TransformComponent>();
            sprite = &entity->getComponent<SpriteComponent>();
            collider = &entity->getComponent<ColliderComponent>();
            projectile = &entity->getComponent<ProjectileComponent>();
            
            transform->velocity = velocity;
            cooldowns = {
                {"Attack", 1000},
                {"Hit", 500}
            };
        }

        void update() override
        {
            float length = sqrt(pow(Game::playerRect.x - transform->position.x, 2) + pow(Game::playerRect.y - transform->position.y, 2));
            float dx = (Game::playerRect.x - transform->position.x) / length;
            float dy = (Game::playerRect.y - transform->position.y) / length;

            float backLength = sqrt(pow(original_vector.x - transform->position.x, 2) + pow(original_vector.y - transform->position.y, 2));
            float back_dx = (original_vector.x - transform->position.x) / backLength;
            float back_dy = (original_vector.y - transform->position.y) / backLength;

            colliderRect.x = transform->position.x;
            colliderRect.y = transform->position.y;
            colliderRect.w = transform->width * transform->scale;
            colliderRect.h = transform->height * transform->scale;

            hit = false;
            // Move to the player
            if (Game::playerRect.x > transform->position.x - range
                && Game::playerRect.x < transform->position.x + range
                && Game::playerRect.y > transform->position.y - range
                && Game::playerRect.y < transform->position.y + range) 
            {
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
                    attacking = true;
                    if (dx > 0) {
                        sprite->spriteFlip = SDL_FLIP_NONE;
                    } else {
                        sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                    }
                        transform->velocity.x = 0;
                        transform->velocity.y = 0;
                            
                            sprite->Play("Attack");
                            attacking = true;
                            currentFrame++;
                            lastick = SDL_GetTicks();
                } else {
                    attacking = false;
                }

                inRange = true;

            } else {
                transform->velocity.x = 0;
                transform->velocity.y = 0;
                sprite->Play("Idle");
                inRange = false;
            }

            // Di chuyển về địa chỉ gốc
            if(transform->position.x != original_vector.x && transform->position.y != original_vector.y && !inRange && !hit)
            {
                transform->velocity.x = back_dx*0.1;
                transform->velocity.y = back_dy*0.1;
                if (back_dx > 0) {
                    sprite->spriteFlip = SDL_FLIP_NONE;
                    sprite->Play("Run");
                } else if (back_dx < 0) {
                    sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                    sprite->Play("Run");
                } else
                {
                sprite->Play("Idle");
                }
            }
            
            // 
            if (Collision::AAABB(colliderRect, Game::playerRect) && attacking) 
            {
                transform->position.x += -dx*5;
                transform->velocity.x = -dx*0.2;
                if (dx > 0) {
                    sprite->spriteFlip = SDL_FLIP_NONE;
                } else {
                    sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                }
                    
                    sprite->Play("Hit");
                    health -= 10;
                    lastick = SDL_GetTicks();
                    std::cout << "Health of Enemy: " << health << std::endl;
            }

            if (health <= 0)
            {
                isDead = true;
            }
            if (isDead)
            {
                entity->destroy();
                std::cout << "Enemy is dead" << std::endl;
            }


        }

    private:
        TransformComponent *transform;
        SpriteComponent *sprite;
        ColliderComponent *collider;
        ProjectileComponent *projectile;

        Vector2D velocity;
        SDL_Rect colliderRect;

        Vector2D original_vector;
        int range = 0;
        int speed = 1;
        int attackRange = 100;
        bool inRange = false;
        bool attacking = false;
        bool hit = false;
        bool isDead = false;
        int health = 100;
        int damage = 10;

};