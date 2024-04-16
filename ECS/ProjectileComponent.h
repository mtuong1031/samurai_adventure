#pragma once

#include "ECS.h"
#include "../Vector2D.h"
#include "Component.h"
#include "../Collision.h"


class ProjectileComponent : public Component {
    public:
        Vector2D pos;

        ProjectileComponent(int rng, int spd, Vector2D vel)
            : range(rng), speed(spd), velocity(vel) {    }

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
            sprite->Play("Idle");
        }
    private:
        TransformComponent *transform;
        SpriteComponent *sprite;
        Vector2D velocity;
        
        int range = 0;
        int speed = 0;
        int distance = 0;
};