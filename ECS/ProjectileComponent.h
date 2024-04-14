#pragma once

#include "ECS.h"
#include "../Vector2D.h"
#include "Component.h"


class ProjectileComponent : public Component {
    public:
        ProjectileComponent(int rng, int spd, Vector2D vel)
            : range(rng), speed(spd), velocity(vel) {    }

        ~ProjectileComponent() 
        {    }

        void init() override {
            transform = &entity->getComponent<TransformComponent>();
            transform->velocity = velocity;
        }

        void update() override {
            distance += speed;

            if (distance > range)
                entity->destroy();
            else if (transform->position.x > Game::camera.x + Game::camera.w ||
                    transform->position.x < Game::camera.x ||
                    transform->position.y > Game::camera.y + Game::camera.h ||
                    transform->position.y < Game::camera.y)
                        entity->destroy();
        }
    private:
        TransformComponent *transform;
        Vector2D velocity;
        
        int range = 0;
        int speed = 0;
        int distance = 0;
};