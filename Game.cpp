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

SDL_Rect Game::camera = { 0, 0, 960, 640 };

Map* map;

std::vector<ColliderComponent*> Game::colliders;

bool Game::isRunning = false;

auto& player(manager.addEntity());
auto& wall(manager.addEntity());   

const char* mapFile = "assets/basemaps-sheet.png";

// tạo nhóm cho các thành phần
enum groupLabels : std::size_t
{
    groupMap,
    groupPlayers,
    groupEnemies,
    groupColliders,
    groupProjectiles
};

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));

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
    Map::LoadMap("assets/map_map.txt", 30, 20 );

    player.addComponent<TransformComponent>(2);
    player.addComponent<SpriteComponent>("image/player_anie.png", true);
    player.addComponent<KeyboardControler>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);
 
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

    camera.x = player.getComponent<TransformComponent>().position.x - 480;
    camera.y = player.getComponent<TransformComponent>().position.y - 320;
    
    if(camera.x < 0)
    {
        camera.x = 0;
    }
    if(camera.y < 0)
    {
        camera.y = 0;
    }
    if(camera.x > camera.w)
    {
        camera.x = camera.w;
    }
    if(camera.y > camera.h)
    {
        camera.y = camera.h;
    }

}

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

void Game::AddTile(int srcX, int srcY, int xpos, int ypos)
{
    auto& tile(manager.addEntity());
    tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, mapFile);
    tile.addGroup(groupMap);
}