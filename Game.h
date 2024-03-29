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
        static bool isRunning;
        static SDL_Rect camera;
        // tạo nhóm cho các thành phần
        enum groupLabels : std::size_t
        {
            groupMap,
            groupPlayers,
            groupColliders,
        };

    private:
        int cnt = 0;
        SDL_Window* window;

};

#endif  // _GAME_H_