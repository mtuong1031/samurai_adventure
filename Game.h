#pragma once

// #include "CommonFunc.h"
#include <iostream>
#include <vector>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

static const int WINDOW_WIDTH = 960;
static const int WINDOW_HEIGHT = 540;

class ColliderComponent;
class AssetManager;

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
        bool Running() 
            { return isRunning;}

        static SDL_Renderer* renderer;
        static SDL_Event event;
        static bool isRunning;
        static SDL_Rect camera;
        static AssetManager* assets;
        // tạo nhóm cho các thành phần
        enum groupLabels : std::size_t
        {
            groupMap,
            groupPlayers,
            groupColliders,
            groupProjectiles,
            groupEnemies,
        };

    private:
        int cnt = 0;
        SDL_Window* window;

};
