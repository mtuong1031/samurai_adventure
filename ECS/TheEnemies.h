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
            
            transform->velocity = velocity;
            cooldowns = {
                {"Attack", 1000},
                {"Hit", 500}
            };
        }

        void MovetoPlayer(float length, float dx, float dy)
        {
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
                inRange = true;

            } else {
                transform->velocity.x = 0;
                transform->velocity.y = 0;
                sprite->Play("Idle");
                inRange = false;
            }
        }

        void attack(float length, float dx, float dy)
        {
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
                    if (Game::playerRect.y + Game::playerRect.h > transform->position.y
                        && Game::playerRect.y < transform->position.y + transform->height) 
                    {
                        transform->velocity.x = 0;
                        transform->velocity.y = 0;
                        sprite->Play("Attack");
                        attacking = true;
                        currentAnimation = "Attack";
                        lastick = SDL_GetTicks();
                    }
                inRange = true;
            } else {
                transform->velocity.x = 0;
                transform->velocity.y = 0;
                sprite->Play("Idle");
                inRange = false;
            }
        }

        void Hit(SDL_Rect colliderRect, float dx) {
            if (Collision::AAABB(colliderRect, Game::playerRect)) 
            {
                transform->position.x += -dx*2;
                transform->velocity.x = -dx*0.1;
                if (dx > 0) {
                    sprite->spriteFlip = SDL_FLIP_NONE;
                } else {
                    sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                }
                sprite->Play("Hit");
                health -= 0;
                lastick = SDL_GetTicks();
                std::cout << "Health of Enemy: " << health << std::endl;
            }
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

            if (length <= range) {
                MovetoPlayer(length, dx, dy);
                if (length <= attackRange) {
                    attack(length, dx, dy);
                    Hit(colliderRect, dx);
                }
            } else {
                transform->velocity.x = back_dx * 0.1;
                transform->velocity.y = back_dy * 0.1;
                if (back_dx > 0) {
                    sprite->spriteFlip = SDL_FLIP_NONE;
                } else {
                    sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
                }
                sprite->Play("Run");
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