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

// tạo nhóm cho các thành phần
enum groupLabels : std::size_t
{
    groupMap,
    groupPlayers,
    groupEnemies,
    groupColliders,
    groupProjectiles
};


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
    map->LoadMap("assets/map.map",16, 20);

    player.addComponent<TransformComponent>(2);
    player.addComponent<SpriteComponent>("image/player_anie.png", true);
    player.addComponent<KeyboardControler>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);

    // thực hiện khởi tạo các thành phần của wall
    wall.addComponent<TransformComponent>(300.0f, 300.0f, 300, 20, 1);
    wall.addComponent<SpriteComponent>("assets/base5.png");
    wall.addComponent<ColliderComponent>("wall");      
    wall.addGroup(groupMap);    
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
        Collision::AAABB(player.getComponent<ColliderComponent>(), *cc);
    }
}

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));

void Game::Render() {
    SDL_RenderClear(renderer);
    // thực hiện vẽ map
    for (auto& t : tiles) {
        t->Draw();
    }
    // thực hiện vẽ player
    for (auto& p : players) {
        p->Draw();
    }
    // thực hiện vẽ ememies
    for (auto& e : enemies) {
        e->Draw();
    }
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

void Game::AddTile(int id, int x, int y)
{
    auto& tile(manager.addEntity());
    tile.addComponent<TileComponent>(x, y, 32, 32, id);
    tile.addGroup(groupMap);
}