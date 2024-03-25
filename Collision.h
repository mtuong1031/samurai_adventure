#ifndef _Collision_h_
#define _Collision_h_

#include "SDL2/SDL.h"

class ColliderComponent;

class Collision 
{
    public:
        static bool AAABB(const SDL_Rect& rect1, const SDL_Rect& rect2);
        static bool AAABB(const ColliderComponent& colA, const ColliderComponent& colB);
};

#endif // _Collision_h_