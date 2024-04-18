#pragma once

#include "ECS.h"
#include "../Vector2D.h"
#include "Component.h"
#include "../Collision.h"


class ProjectileComponent : public Component {
    public:
        Vector2D pos;
        std::string id;
        int index = 0;
        Uint32 lastTime = 0;

        ProjectileComponent(int rng, int spd, Vector2D vel, std::string id)
        { 
            range = rng;
            speed = spd;
            velocity = vel;
            this->id = id;
        }

        ProjectileComponent(int rng, int spd, Vector2D vel, std::string id, int index)
        {
            range = rng;
            speed = spd;
            velocity = vel;
            this->id = id;
            this->index = index;
        }

        ProjectileComponent(Vector2D vel)
        {
            pos.x = vel.x;
            pos.y = vel.y;
        }

        ~ProjectileComponent() 
        {    }

        void init() override {
            transform = &entity->getComponent<TransformComponent>();
            sprite = &entity->getComponent<SpriteComponent>();
            transform->velocity = velocity;
        }

        void update() override {
            if (id == "hp") {
                sprite->Play("Idle");
                transform->velocity.x = velocity.x * 0.5;
                transform->velocity.y = velocity.y * 0.5;
            } 
            if (id == "effect") {
                if (index == 1) {
                    sprite->Play("Hit");
                } else {
                    sprite->Play("Hit");
                }
                
            }
            if (id == "skill") {
                if (index == 1) {
                    sprite->Play("Idle");
                }
                if (index == 2) {
                    sprite->Play("Hit");
                }
            }
            if (id == "skill_of_player") {
                sprite->Play("Idle");
                distance += speed;
                if (distance > range) {
                    entity->destroy();
                }
            }
        }
    private:
        TransformComponent *transform;
        SpriteComponent *sprite;
        Vector2D velocity;
    
        int range = 0;
        int speed = 0;
        int distance = 0;
};