#include "Game.h"

const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 672;

Game *game = nullptr;

int main (int argc, char *argv[]) {\

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    game = new Game();

    game->Init("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);
    game->Menu();

    while (game->Running())
    {

        frameStart = SDL_GetTicks();

        game->HandleEvents();
        game->Update();
        game->Render();

        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime) 
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    game->Clean();

    return 0;
}