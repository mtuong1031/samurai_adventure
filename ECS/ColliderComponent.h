#ifndef _ColliderComponent_h_
#define _ColliderComponent_h_

#include "Component.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "ECS.h"
#include "SDL2/SDL.h"
#include "string"


//Quản lí va chạm giữa các đối tượng
class ColliderComponent : public Component
{
    public:
        SDL_Rect collider;
        std::string tag;

        TransformComponent *transform;

        ColliderComponent(std::string t)
        {
            tag = t;
        }
        void init() override
        {
            if (!entity->hasComponent<TransformComponent>()){
                entity->addComponent<TransformComponent>();
            }
            transform = &entity->getComponent<TransformComponent>();

            Game::colliders.push_back(this);
        }
    
        void update() override 
        {
            collider.x = static_cast<int>(transform->position.x);
            collider.y = static_cast<int>(transform->position.y);
            collider.w = transform->width * transform->scale;
            collider.h = transform->height * transform->scale;
        }

    private:
    
};

#endif // _ColliderComponent_H_