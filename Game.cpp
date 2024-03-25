#include "Game.h"   
#include "TextureManager.h"
#include "Map.h"
#include "SDL2/SDL.h"
#include "ECS/ECS.h"
#include "ECS/Component.h"
#include "ECS/TransformComponent.h"
#include "Vector2D.h" 
#include "Collision.h"



Game::Game() {
}
Game::~Game() {
}

Manager manager;
SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;
Map* map;

std::vector<ColliderComponent*> Game::colliders;

auto& player(manager.addEntity());
auto& wall(manager.addEntity());    

auto& tile1(manager.addEntity());
auto& tile2(manager.addEntity());
auto& tile3(manager.addEntity());

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

    // thực hiện khởi tạo các thành phần của tile
    tile1.addComponent<TileComponent>(200, 200, 32, 32, 0);
    tile2.addComponent<TileComponent>(250, 250, 32, 32, 1);
    tile2.addComponent<ColliderComponent>("base6"); 
    tile3.addComponent<TileComponent>(300, 300, 32, 32, 2);
    tile3.addComponent<ColliderComponent>("base7");    

    // thực hiện khởi tạo các thành phần của player
    player.addComponent<TransformComponent>(2);
    player.addComponent<SpriteComponent>("assets/base1.png");
    player.addComponent<KeyboardControler>();
    player.addComponent<ColliderComponent>("player");

    // thực hiện khởi tạo các thành phần của wall
    wall.addComponent<TransformComponent>(300.0f, 300.0f, 300, 20, 1);
    wall.addComponent<SpriteComponent>("assets/base5.png");
    wall.addComponent<ColliderComponent>("wall");      

}

void Game::HandleEvents() {


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

    for (auto cc : colliders) 
    {
        if (Collision::AAABB(player.getComponent<ColliderComponent>(), *cc->collider);
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