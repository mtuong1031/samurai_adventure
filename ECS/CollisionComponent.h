#ifndef _CollisionComponent_h_
#define _CollisionComponent_h_

#include "Component.h"
#include "ECS.h"
#include "SDL2/SDL.h"
#include "string"

class CollisionComponent : public Component
{
        SDL_Rect collider;
        std::string tag;

        TransformComponent *transform;

        void init() override
        {
            if (!entity->hasComponent<TransformComponent>()){
                entity->addComponent<TransformComponent>();
            }
            transform = &entity->getComponent<TransformComponet>();
        }
    
    void update() override 
    {
        collider.x = transform->position.x;
        collider.y = transform->position.y;
        collider.w = transform->width * transform->scale;
        collider.h = transform->height * transform->scale;
    }

};

#endif // _CollisionComponent_H_