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
        TransformComponent *transform;
        SpriteComponent *sprite;
        ColliderComponent *collider;
        ProjectileComponent *projectile;

        Uint32 lastick = 0;
        std::unordered_map<const char*, int> cooldowns; 
        int currentFrame = 0;
        bool attack_frame = false;
        const char* currentAnimation;

        int health = 100;
        int damage = 10;
        bool hit = false;
        bool isAttacking;

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
            isAttacking = false;
            cooldowns = {
                {"Attack", 1000},
                {"Hit", 500}
            };
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
            float length = sqrt(pow(Game::playerRect.x - transform->position.x, 2) + pow(Game::playerRect.y - transform->position.y, 2));
            float dx = (Game::playerRect.x - transform->position.x) / length;
            float dy = (Game::playerRect.y - transform->position.y) / length;

            float backLength = sqrt(pow(original_vector.x - transform->position.x, 2) + pow(original_vector.y - transform->position.y, 2));
            float back_dx = (original_vector.x - transform->position.x) / backLength;
            float back_dy = (original_vector.y - transform->position.y) / backLength;

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
                        // if (SDL_GetTicks() - lastick >= cooldowns["Attack"]) {
                            sprite->Play("Attack");
                            
                            currentFrame++;
                            lastick = SDL_GetTicks();
                        // }
                }
            } else {
                transform->velocity.x = 0;
                transform->velocity.y = 0;
                sprite->Play("Idle");
                inRange = false;
                isAttacking = false;
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

            if (hit) {
                sprite->Play("Hit");
                transform->position.x -= transform->velocity.x * 2;
                transform->position.y -= transform->velocity.y * 2;
                std::cout << "Hit  " << std::endl;
                hit = false;
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

            if (currentFrame >= sprite->animations[sprite->currentAnimationName].frames)
            {
                currentFrame = 0;
                sprite->Play("Idle");
                attack_frame = false;
            } else attack_frame = true;
        }

    private:
        Vector2D velocity;
        SDL_Rect colliderRect;

        Vector2D original_vector;
        int range = 0;
        int speed = 1;
        int attackRange = 100;
        bool inRange = false;
        bool isDead = false;
};