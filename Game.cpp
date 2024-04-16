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
SDL_Rect Game::camera = {0, 0, 1279, 0};
SDL_Rect Game::playerRect = {0, 0, 64, 80};  // x y w h
SDL_Rect background = {0, 0, 1279, 540};

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
    assets->AddTexture("background", "image/ParallaxBackground.png");
    // assets->AddTexture("player", "image/ani_sd.png");
    assets->AddTexture("player", "image/tuong.png");
    assets->AddTexture("arrow", "image/arrow.png");
    assets->AddTexture("hp", "image/hp.png");
    assets->AddTexture("enemy", "image/enemy1_ani.png");

    // map = new Map("terrain", 1, 32);
    // map->LoadMap("assets/map.map", 30, 20);
    map = new Map("terrain", 1,48); 
    map->LoadMap("assets/2s.map", 55, 16);

    // thực hiện khởi tạo các thành phần của player
    assets->CreatePlayer(Vector2D(300, 300), 200, "player");
 
    assets->CreateProjectile(Vector2D(300, 100), Vector2D(0, 0), 200, 2, "hp");
    assets->CreateProjectile(Vector2D(100, 300), Vector2D(0, 0), 200, 2, "hp");
    assets->CreateProjectile(Vector2D(300, 400), Vector2D(0, 0), 200, 2, "hp");
    assets->CreateProjectile(Vector2D(200, 300), Vector2D(0, 0), 200, 2, "hp");
    // thực hiện khởi tạo các thành phần của enemy
    assets->CreateEnemies(Vector2D(600, 100), Vector2D(0,0), 200, 1, "enemy");
    assets->CreateEnemies(Vector2D(300, 100), Vector2D(0,0), 200, 1, "enemy");
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

    for (auto& p : projectiles) {
        if (Collision::AAABB(playerCol, p->getComponent<ColliderComponent>().collider))
        {
            std::cout << "Player hit projectile" << std::endl;
        }
    }

    if (players[0]->getComponent<KeyboardControler>().health <= 0)
    {
        players[0]->destroy();
        std::cout << "Player is dead" << std::endl;
    }

    Vector2D bullet_vel(0, 0);
    bullet_vel = players[0]->getComponent<KeyboardControler>().BulletVel;
    assets->CreateProjectile(playerPos, bullet_vel, 200, 2, "hp");

    // camera.x = player.getComponent<TransformComponent>().position.x - 480;
    // camera.y = player.getComponent<TransformComponent>().position.y - 320;
    camera.x = players[0]->getComponent<TransformComponent>().position.x - 480;
    camera.y = players[0]->getComponent<TransformComponent>().position.y - 320;

    playerRect.w = players[0]->getComponent<SpriteComponent>().playersRect.w;
    playerRect.h = players[0]->getComponent<SpriteComponent>().playersRect.h;  
    playerRect.x = players[0]->getComponent<SpriteComponent>().playersRect.x;
    playerRect.y = players[0]->getComponent<SpriteComponent>().playersRect.y;

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

    TextureManager::Draw(assets->GetTexture("background"), background, background, SDL_FLIP_NONE);
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

    for (auto& p : projectiles) {
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
