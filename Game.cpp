#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Component.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"

Manager manager;
Map* map;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

// SDL_Rect Game::camera = {0, 0, map->getScaledSize() * map->getSizeX() - WINDOW_WIDTH, 
//                                 map->getScaledSize() * map->getSizeY() - WINDOW_HEIGHT};
SDL_Rect Game::camera = {0, 0, 960, 0};
SDL_Rect Game::playerRect = {0, 0, 64, 80};  // x y w h

AssetManager* Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;

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

    assets->AddTexture("terrain", "assets/tileset.png");
    // assets->AddTexture("player", "image/ani_sd.png");
    assets->AddTexture("player", "image/animatinons_char1.png");
    assets->AddTexture("projectile", "image/proj.png");\
    assets->AddTexture("enemy", "image/ani_sd.png");

    // map = new Map("terrain", 1, 32);
    // map->LoadMap("assets/map.map", 30, 20);
    map = new Map("terrain", 1,48); 
    map->LoadMap("assets/2s.map", 55, 16);

    // thực hiện khởi tạo các thành phần của player
    assets->CreatePlayer(Vector2D(300, 300), 200, "player");
 
    // thực hiện khởi tạo các thành phần của enemy
    assets->CreateEnemies(Vector2D(600, 100), Vector2D(0,0), 200, 1, "enemy");
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& enemies(manager.getGroup(Game::groupEnemies));

void Game::HandleEvents() 
{

    SDL_PollEvent(&event);
    switch (event.type) {
    case SDL_QUIT:
        isRunning = false;
        break;
    default:
        break;
    }
}

void Game::Update() 
{

    // SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
    // Vector2D playerPos = player.getComponent<TransformComponent>().position;
    SDL_Rect playerCol = players[0]->getComponent<ColliderComponent>().collider;
    Vector2D playerPos = players[0]->getComponent<TransformComponent>().position;

    manager.refresh();
    manager.update();

    for (auto& c : colliders) {
        SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
        
        if (Collision::AAABB(playerCol, cCol))
        {
            playerPos = playerPos - players[0]->getComponent<TransformComponent>().velocity;
            players[0]->getComponent<TransformComponent>().position = playerPos;
        }
    }

    for (auto& e : enemies) {
        SDL_Rect eCol = e->getComponent<ColliderComponent>().collider;
        Vector2D ePos = e->getComponent<TransformComponent>().position;
        Vector2D eVel = e->getComponent<TransformComponent>().velocity;
        
        if (Collision::AAABB(playerCol, eCol))
        {
            std::cout << "Player hit enemy" << std::endl;
        }

    }

    // camera.x = player.getComponent<TransformComponent>().position.x - 480;
    // camera.y = player.getComponent<TransformComponent>().position.y - 320;
    camera.x = players[0]->getComponent<TransformComponent>().position.x - 480;
    camera.y = players[0]->getComponent<TransformComponent>().position.y - 320;
    playerRect.x = players[0]->getComponent<TransformComponent>().position.x;
    playerRect.y = players[0]->getComponent<TransformComponent>().position.y;
    
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

    // for (auto& c : colliders)
    // {
    //     c->Draw();
    // }

    // thực hiện vẽ player
    for (auto& p : players) {
        p->Draw();
    }

    for (auto& e : enemies)
    {
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
