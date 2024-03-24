#include "Game.h"   
#include "TextureManager.h"
#include "Map.h"

#include "ECS/ECS.h"
#include "ECS/Component.h"
#include "ECS/PositionComponents.h"

Map* map;  

SDL_Renderer* Game::renderer = nullptr;

Manager manager;
auto& player(manager.addEntity());

Game::Game() {
}
Game::~Game() {
}

void Game::Init(const char *tiles, int xpos, int ypos, int width, int height, bool fullscreen) {
    int flags = 0;

    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        window = SDL_CreateWindow(tiles, xpos, ypos, width, height, flags);
        if (window) {
            std::cout << "Window created!" << std::endl;
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer) {
            std::cout << "Renderer created!" << std::endl;
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        isRunning = true;
    } else {
        isRunning = false;
    }
    map = new Map();

    // thực hiện khởi tạo các thành phần của player
    player.addComponent<PositionComponent>();
    player.addComponent<SpriteComponent>("assets/base1.png");
    
}

void Game::HandleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
    case SDL_QUIT:
        isRunning = false;
        break;
    default:
        break;
    }
}

void Game::Update() {
    
    manager.refresh();
    manager.update();

    if (player.getComponent<PositionComponent>().x() > 100) {
        player.getComponent<SpriteComponent>().setTex("assets/base9.png");
    }

}

void Game::Render() {
    SDL_RenderClear(renderer);
    map->DrawMap();
    manager.draw();
    SDL_RenderPresent(renderer);
}

void Game::Clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned!" << std::endl;
}

bool Game::Running() {
    return isRunning;
}