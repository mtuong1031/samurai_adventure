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
SDL_Rect Game::camera = {0, 0, 1200, 0};
SDL_Rect Game::playerRect = {0, 0, 64, 80};  // x y w h
SDL_Rect background = {0, 0, 1297, 540};

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
    assets->AddTexture("player", "image/tuong.png");
    assets->AddTexture("arrow", "image/arrow.png");
    assets->AddTexture("hp", "image/hp.png");
    assets->AddTexture("enemy", "image/enemy1_ani.png");
    assets->AddTexture("effect", "image/skill.png");

    // map = new Map("terrain", 1, 32);
    // map->LoadMap("assets/map.map", 30, 20);
    map = new Map("terrain", 1,48); 
    map->LoadMap("assets/2s.map", 55, 14);

    // thực hiện khởi tạo các thành phần của player
    assets->CreatePlayer(Vector2D(300, 300), 200, "player");
 
    assets->CreateProjectile(Vector2D(300, 100), Vector2D(0, 0), 200, 2, "hp");
    assets->CreateProjectile(Vector2D(100, 300), Vector2D(0, 0), 200, 2, "hp");
    // thực hiện khởi tạo các thành phần của enemy
    assets->CreateEnemies(Vector2D(600, 100), Vector2D(0,0), 200, 1, "enemy");
    assets->CreateEnemies(Vector2D(300, 100), Vector2D(0,0), 200, 1, "enemy");
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& effects(manager.getGroup(Game::groupEffects));

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
    SDL_Rect playerCol = players[0]->getComponent<ColliderComponent>().collider;
    Vector2D playerPos = players[0]->getComponent<TransformComponent>().position;
    ColliderComponent playerCollider = players[0]->getComponent<ColliderComponent>();

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

    // Xử lí các thành phần của Enemy
    //////////////////////////////////////////
    for (auto& e : enemies) {
        SDL_Rect eCol = e->getComponent<ColliderComponent>().collider;
        Vector2D ePos = e->getComponent<TransformComponent>().position;
        Vector2D eVel = e->getComponent<TransformComponent>().velocity;

        // ePos.x -= 187 - 32;
        // ePos.y -= 60 - 32;
        Vector2D pVel = e->getComponent<TheEnemies>().getDirection(playerPos);
        bool isAttack = e->getComponent<TheEnemies>().isAttacking;
        if (SDL_GetTicks() - e->getComponent<TheEnemies>().lastick >= 4000 && isAttack)
        {
            isAttack = true;
            e->getComponent<TheEnemies>().lastick = SDL_GetTicks();
        } else {
            isAttack = false;
        }
        if (isAttack) {
            assets->CreateProjectile(ePos, pVel, 200, 1, "hp");
        }
        for (auto ef : effects) {
            if (Collision::AAABB(eCol, ef->getComponent<ColliderComponent>().collider))
            {
                e->getComponent<TheEnemies>().health -= 1;
                e->getComponent<TheEnemies>().hit = true;
                std::cout << "Enemy health: " << e->getComponent<TheEnemies>().health << std::endl;
            }
        } 
    }

    // Xử lí lớp đạn
    for (auto& p : projectiles) {
        if (Collision::AAABB(playerCol, p->getComponent<ColliderComponent>().collider))
        {
            players[0]->getComponent<KeyboardControler>().health -= 5;
            std::cout << "Player health: " << players[0]->getComponent<KeyboardControler>().health << std::endl;
            p->destroy();
        }
    }


    // Xử lý kĩ năng của nhân vật
    SDL_RendererFlip flip = players[0]->getComponent<SpriteComponent>().spriteFlip;
    bool isSkill = players[0]->getComponent<KeyboardControler>().isAttacking;
    Vector2D skillRect;
    if (SDL_GetTicks() - players[0]->getComponent<KeyboardControler>().lastTick >= 25 && isSkill)
    {
        if (flip == SDL_FLIP_HORIZONTAL) 
            skillRect = playerPos - Vector2D(57,0);
        else 
            skillRect = playerPos + Vector2D(57,0);

        assets->CreateEffect(playerPos, Vector2D(0, 0), 200, 2, "effect", flip);
        players[0]->getComponent<KeyboardControler>().lastTick = SDL_GetTicks();
    } else {
        isSkill = false;
        for (auto& ef : effects) {
            ef->destroy();
        }
    }

    // ss
    if (players[0]->getComponent<KeyboardControler>().health <= 0)
    {
        players[0]->destroy();
        std::cout << "Player is dead" << std::endl;
    }

    Vector2D bullet_vel(0, 0);
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

    for (auto& e : effects) {
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
