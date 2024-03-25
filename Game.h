#ifndef _GAME_H_
#define _GAME_H_

#include "CommonFunc.h"
#include <vector>

class ColliderComponent;

class Game 
{
    public:
        Game();
        ~Game();

        void Init (const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
        void HandleEvents();
        void Update();
        void Render();
        void Clean();
        bool Running();

        static SDL_Renderer* renderer;
        static SDL_Event event;
        static std::vector<ColliderComponent*> colliders;

    private:
        int cnt = 0;
        SDL_Window* window;
        bool isRunning;
};

#endif  // _GAME_H_