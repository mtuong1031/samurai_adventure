#ifndef _GAME_H_
#define _GAME_H_

#include "CommonFunc.h"

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

    private:
        int cnt;
        SDL_Window* window;
        SDL_Renderer* renderer;
        bool isRunning;
};

#endif  // _GAME_H_