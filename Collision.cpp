#include "Collision.h"
#include "ECS/ColliderComponent.h"  

bool Collision::AAABB(const SDL_Rect& rect1, const SDL_Rect& rect2)
{
    if (rect1.x + rect1.w >= rect2.x &&
        rect2.x + rect2.w >= rect1.x &&
        rect1.y + rect1.h >= rect2.y &&
        rect2.y + rect2.h >= rect1.y)
    {
        return true;
    }

    return false;
}

bool Collision::AAABB(const ColliderComponent& col1, const ColliderComponent& col2)
{
    if (AAABB(col1.collider, col2.collider))
    {
        std::cout << col1.tag << " hit: " << col2.tag << std::endl;
        return true;
    }
    else 
    {
        return false;
    }   
}