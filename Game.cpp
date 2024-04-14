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

AssetManager* Game::assets = new AssetManager(&manager);

bool Game::isRunning = false;

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

    player.addComponent<TransformComponent>(1);
    player.addComponent<SpriteComponent>("player", true);
    player.addComponent<KeyboardControler>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);
 
    // thực hiện khởi tạo các thành phần của enemy
    // assets->CreateEnemy(Vector2D(300, 300), 200, 2, "enemy");   
    // assets->CreateEnemy(Vector2D(400, 300), 200, 2, "enemy"); 

    // assets->CreateProjectile(Vector2D(600, 600), Vector2D(2, 0), 200, 2, "projectile");
    // assets->CreateProjectile(Vector2D(600, 620), Vector2D(2, 0), 200, 2, "projectile");
    // assets->CreateProjectile(Vector2D(400, 600), Vector2D(2, 1), 200, 2, "projectile");
    // assets->CreateProjectile(Vector2D(600, 600), Vector2D(2, -1), 200, 2, "projectile");
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
// auto& enemies(manager.getGroup(Game::groupEnemies));

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

    SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
    Vector2D playerPos = player.getComponent<TransformComponent>().position;
    
    manager.refresh();
    manager.update();

    for (auto& c : colliders) {
        SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
        
        if (Collision::AAABB(playerCol, cCol))
            player.getComponent<TransformComponent>().position.y = playerPos.y;
    }

    // for (auto& p : projectiles) {
    //     SDL_Rect progCol = p->getComponent<ColliderComponent>().collider;
        
    //     if (Collision::AAABB(playerCol, progCol))
    //         p->destroy();
    // }

    for (auto& e : enemies) {
        SDL_Rect eCol = e->getComponent<ColliderComponent>().collider;
        
        if (Collision::AAABB(playerCol, eCol))
            e->destroy();
    }

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

    // for (auto& c : colliders)
    // {
    //     c->Draw();
    // }

    // thực hiện vẽ player
    for (auto& p : players) {
        p->Draw();
    }

    for (auto& p : projectiles)
    {
        p->Draw();
    }

    // for (auto& e : enemies)
    // {
    //     e->Draw();
    // }

    SDL_RenderPresent(renderer);
}

void Game::Clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout << "Game cleaned!" << std::endl;
}