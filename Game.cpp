#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Component.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <sstream>

int s = 2;
Map* map;
Manager manager;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

// SDL_Rect Game::camera = {0, 0, map->getScaledSize() * map->getSizeX() - WINDOW_WIDTH, 
//                                 map->getScaledSize() * map->getSizeY() - WINDOW_HEIGHT};
SDL_Rect Game::camera = {0, 0, 960, 768};
SDL_Rect Game::playerRect = {0, 0, 64, 80};  // x y w h

AssetManager* Game::assets = new AssetManager(&manager);
Mix_Chunk* Game::hitSound;
Mix_Chunk* Game::attackSound;
Mix_Chunk* Game::runSound;
Mix_Chunk* Game::enemy_hit;
Mix_Chunk* Game::main_hit;
Mix_Music* Game::bgm;


bool Game::isRunning = false;

auto& label(manager.addEntity());

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

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "Error: " << Mix_GetError() << std::endl;
    }

    if (TTF_Init() == -1) {
        std::cout << "Error: " << std::endl;
    }

    bgm = Mix_LoadMUS("sound/bgm.wav");
    Mix_PlayMusic(bgm, -1);
    hitSound = Mix_LoadWAV("sound/hit.wav");
    attackSound = Mix_LoadWAV("sound/attack.wav");
    runSound = Mix_LoadWAV("sound/run.wav");
    enemy_hit = Mix_LoadWAV("sound/enemy_hit.wav");
    main_hit = Mix_LoadWAV("sound/main_hit.wav");

    assets->AddTexture("terrain", "assets/tileset.png");
    assets->AddTexture("player", "image/tuong.png");
    assets->AddTexture("arrow", "image/arrow.png");
    assets->AddTexture("hp", "image/fire.png");
    assets->AddTexture("enemy", "image/enemy1_ani.png");

    assets->AddTexture("effect", "image/skill.png");
    assets->AddTexture("attack_up", "image/tuong_attack_up.png");
    assets->AddTexture("attack_down", "image/tuong_attack_down.png");

    assets->AddTexture("boss", "image/boss_anie.png");
    assets->AddTexture("skill", "image/attack_boss.png");
    assets->AddTexture("sb1", "image/boss_effect_2.png");

    assets->AddTexture("ssss", "image/ssss.png");
    assets->AddTexture("skill2", "image/skill2.png");
    assets->AddTexture("chest", "image/chest.png");

    assets->AddFont("arial", "assets/arial.ttf", 16);

    map = new Map("terrain", 1,48); 
    map->LoadMap("assets/3s.map", 40, 30);

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color red = {255, 0, 0, 255};

    label.addComponent<UILabel>(55, 33, "Player HP: 100", "arial", white);

    // thực hiện khởi tạo các thành phần của player
    assets->CreatePlayer(Vector2D(300, 300), 200, "player");
    assets->CreateItem(Vector2D(500, 300), Vector2D(0, 0), 500, 1, "chest", SDL_Rect{0, 0, 48, 48});
}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& enemies(manager.getGroup(Game::groupEnemies));

auto& effects(manager.getGroup(Game::groupEffects));
auto& skills(manager.getGroup(Game::groupSkills));
auto& bosses(manager.getGroup(Game::groupBosses));
auto& sssses(manager.getGroup(Game::groupSssses));
auto& blooks(manager.getGroup(Game::groupBlooks));
auto& items(manager.getGroup(Game::groupItems));

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

bool Game::Menu() 
{
    assets->AddTexture("menu", "image/menu.png");
    SDL_Rect srcRect, destRect;
    srcRect.x = srcRect.y = 0;
    srcRect.w = 960;
    srcRect.h = 672;
    destRect.x = destRect.y = 0;
    destRect.w = 960;
    destRect.h = 672;

    TextureManager::Draw(assets->GetTexture("menu"), srcRect, destRect, SDL_FLIP_NONE);
    
    bool ret = false;
    bool isRuning = true;
    SDL_Event e;
    int x = 0, y = 0;
    
    while (isRuning)
    {
        while(SDL_PollEvent(&e)) 
        {
            if (e.type == SDL_QUIT) 
            {
                isRuning = false;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) 
            {
                SDL_GetMouseState(&x, &y);
                if (x >= 0 && x <= 960 && y >= 0 && y <= 672) 
                {
                    isRuning = false;
                    ret = true;
                }
            } else if (e.type == SDL_KEYDOWN) 
            {
                isRuning = false;
                ret = true;
                SDL_DestroyTexture(assets->GetTexture("menu")); 
            }
        } 
        SDL_RenderPresent(renderer);
    }
    return ret;
}

void Game::CrateHpBar() 
{
    SDL_Rect playerhealth = {0, 0, 150, 36};
    int lastPLayerHealthWidth = playerhealth.w - 12;

    const int playerMaxHealth = players[0]->getComponent<KeyboardControler>().max_health;
    int playerCurrentHealth = players[0]->getComponent<KeyboardControler>().health;

    // Player health bar
    SDL_Rect firstLayer = {44, 24, playerhealth.w, playerhealth.h};
    SDL_Rect secondLayer = {50, 30, (playerhealth.w - 12) * playerCurrentHealth / playerMaxHealth, playerhealth.h - 12};

    if (lastPLayerHealthWidth > secondLayer.w)
        lastPLayerHealthWidth--;
    if (secondLayer.w > playerhealth.w) {
        secondLayer.w = playerhealth.w - 12;
    }
    if (secondLayer.w < 12) {
        secondLayer.w = 12;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &firstLayer);

    SDL_SetRenderDrawColor(renderer, 122, 0, 0, 255);
    SDL_RenderFillRect(renderer, &secondLayer);

    if (bosses[0]->getComponent<TheBosses>().inRange) {
        SDL_Rect bosshealth = {0, 0, 400, 24};
        int lastBossHealthWidth = bosshealth.w - 12;

        const int bossMaxHealth = bosses[0]->getComponent<TheBosses>().max_health;
        int bossCurrentHealth = bosses[0]->getComponent<TheBosses>().health;

        // Boss health bar
        SDL_Rect firstLayer = {270, 90, bosshealth.w, bosshealth.h};
        SDL_Rect secondLayer = {276, 96, (bosshealth.w - 12) * bossCurrentHealth / bossMaxHealth, bosshealth.h - 12};

        if (lastBossHealthWidth > secondLayer.w)
            lastBossHealthWidth--;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &firstLayer);

        SDL_SetRenderDrawColor(renderer, 122, 0, 0, 255);
        SDL_RenderFillRect(renderer, &secondLayer);
    }

}

void Game::Update() 
{

    SDL_RendererFlip flip = SDL_FLIP_NONE;

    SDL_Rect playerCol = players[0]->getComponent<ColliderComponent>().collider;
    Vector2D playerPos = players[0]->getComponent<TransformComponent>().position;
    ColliderComponent playerCollider = players[0]->getComponent<ColliderComponent>();
    int dmg = 10;

    std::stringstream ss;
    ss << "Player HP: " << players[0]->getComponent<KeyboardControler>().health;
    label.getComponent<UILabel>().SetLabelText(ss.str(), "arial");

    manager.refresh();
    manager.update();

    for (auto& c : colliders) {
        SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
        
        if (Collision::AAABB(playerCol, cCol))
        {
            playerPos = playerPos - players[0]->getComponent<TransformComponent>().velocity;
            players[0]->getComponent<TransformComponent>().position = playerPos;
        }
        for (auto& p : projectiles) {
            SDL_Rect pCol = p->getComponent<ColliderComponent>().collider;
            if (Collision::AAABB(pCol, cCol))
            {
                p->destroy();
            }
        }
        for (auto& s : skills) {
            SDL_Rect sCol = s->getComponent<ColliderComponent>().collider;
            if (Collision::AAABB(sCol, cCol))
            {
                s->destroy();
            }
        }
    }

    // Xử lí các thành phần của Enemy
    //////////////////////////////////////////
    for (auto& e : enemies) {
        SDL_Rect eCol = e->getComponent<ColliderComponent>().collider;
        Vector2D ePos = e->getComponent<TransformComponent>().position;
        Vector2D eVel = e->getComponent<TransformComponent>().velocity;

        for (auto& c : colliders) {
            SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
            if (Collision::AAABB(eCol, cCol))
            {
                ePos = ePos - eVel*5;
                e->getComponent<TransformComponent>().position = ePos;
            }
        }

        Vector2D pVel = e->getComponent<TheEnemies>().getDirection(playerPos);
        bool isAttack = e->getComponent<TheEnemies>().isAttacking;
        flip = e->getComponent<SpriteComponent>().spriteFlip;
        if (SDL_GetTicks() - e->getComponent<TheEnemies>().lastick >= 5000 && isAttack)
        {
            if (flip == SDL_FLIP_HORIZONTAL) 
                ePos = ePos + Vector2D(32,0);
            else 
                ePos = ePos + Vector2D(32,0);
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
                e->getComponent<TheEnemies>().health -= 10;
                e->getComponent<TheEnemies>().hit = true;
                Mix_PlayChannel(-1, enemy_hit, 0);
            }

            if (e->getComponent<TheEnemies>().hit) {
                e->getComponent<TransformComponent>().position = ePos - eVel*50;
            }
        } 

        for (auto& s : sssses) {
            if (Collision::AAABB(eCol, s->getComponent<ColliderComponent>().collider))
            {
                s->destroy();
                e->getComponent<TheEnemies>().health -= 10;
                e->getComponent<TheEnemies>().hit = true;
                Mix_PlayChannel(-1, enemy_hit, 0);
            } else {
                e->getComponent<TheEnemies>().hit = false;
            }
            if (e->getComponent<TheEnemies>().hit) {
                e->getComponent<TransformComponent>().position = ePos - eVel*50;
            }
        }

    }

    // XỬ LÍ LỚP BOSS
    for (auto& boss : bosses) {
        SDL_Rect bossCol = boss->getComponent<ColliderComponent>().collider;
        Vector2D bossPos = boss->getComponent<TransformComponent>().position;
        Vector2D bossVel = boss->getComponent<TransformComponent>().velocity;
        flip = boss->getComponent<SpriteComponent>().spriteFlip;

        Vector2D pVel = boss->getComponent<TheBosses>().getDirection(playerPos);
        bool isAttack = boss->getComponent<TheBosses>().isAttacking;
        if (SDL_GetTicks() - boss->getComponent<TheBosses>().lastick >= 1000 && isAttack)
        {
            if (flip == SDL_FLIP_HORIZONTAL) 
                bossPos = bossPos + Vector2D(125,110);
            else 
                bossPos = bossPos + Vector2D(125,110);
            isAttack = true;
            assets->CreateSkill(bossPos, pVel, 200, 1, "skill", SDL_FLIP_NONE, SDL_Rect{0, 0, 32, 10}, 2);

            if (boss->getComponent<TheBosses>().status == 1) {
                assets->CreateSkill(bossPos, Vector2D(1,1), 200, 1, "skill", SDL_FLIP_NONE, SDL_Rect{0, 0, 32, 10}, 2);
                assets->CreateSkill(bossPos, Vector2D(-1,1), 200, 1, "skill", SDL_FLIP_NONE, SDL_Rect{0, 0, 32, 10}, 2);
                assets->CreateSkill(bossPos, Vector2D(1,-1), 200, 1, "skill", SDL_FLIP_NONE, SDL_Rect{0, 0, 32, 10}, 2);
            }

            if (boss->getComponent<TheBosses>().status == 2) {
                assets->CreateSkill(bossPos, Vector2D(0,1), 200, 1, "skill", SDL_FLIP_NONE, SDL_Rect{0, 0, 32, 10}, 2);
                assets->CreateSkill(bossPos, Vector2D(-1,0), 200, 1, "skill", SDL_FLIP_NONE, SDL_Rect{0, 0, 32, 10}, 2);
                assets->CreateSkill(bossPos, Vector2D(1,0), 200, 1, "skill", SDL_FLIP_NONE, SDL_Rect{0, 0, 32, 10}, 2);
                assets->CreateSkill(bossPos, Vector2D(0,-1), 200, 1, "skill", SDL_FLIP_NONE, SDL_Rect{0, 0, 32, 10}, 2);
            }
            
            boss->getComponent<TheBosses>().lastick = SDL_GetTicks();
        } else {
            isAttack = false;
        }

        if (boss->getComponent<TheBosses>().status == 1
            && SDL_GetTicks() - boss->getComponent<TheBosses>().lastick >= 80)
        {
            assets->CreateSkill(Vector2D(720,768), Vector2D(0, 1), 200, 1, "sb1", SDL_FLIP_NONE, SDL_Rect{0, 0, 48, 48}, 1);
            assets->CreateSkill(Vector2D(96,768), Vector2D(0, 1), 200, 1, "sb1", SDL_FLIP_NONE, SDL_Rect{0, 0, 48, 48}, 1);
        } else {}

        switch (boss->getComponent<TheBosses>().health) {
            case 750:
                assets->CreateItem(Vector2D(336, 912), Vector2D(0, 0), 200, 1, "chest", SDL_Rect{0, 0, 48, 48});
                assets->CreateItem(Vector2D(576, 912), Vector2D(0, 0), 200, 1, "chest", SDL_Rect{0, 0, 48, 48});
                assets->CreateItem(Vector2D(576, 1200), Vector2D(0, 0), 200, 1, "chest", SDL_Rect{0, 0, 48, 48});
                assets->CreateItem(Vector2D(336, 1200), Vector2D(0, 0), 200, 1, "chest", SDL_Rect{0, 0, 48, 48});
                boss->getComponent<TheBosses>().health -= 3;
                break;
            case 400:
                assets->CreateItem(Vector2D(336, 912), Vector2D(0, 0), 200, 1, "chest", SDL_Rect{0, 0, 48, 48});
                assets->CreateItem(Vector2D(576, 912), Vector2D(0, 0), 200, 1, "chest", SDL_Rect{0, 0, 48, 48});
                assets->CreateItem(Vector2D(576, 1200), Vector2D(0, 0), 200, 1, "chest", SDL_Rect{0, 0, 48, 48});
                assets->CreateItem(Vector2D(336, 1200), Vector2D(0, 0), 200, 1, "chest", SDL_Rect{0, 0, 48, 48});
                boss->getComponent<TheBosses>().health -= 3;
                break;
            case 100:
                assets->CreateItem(Vector2D(336, 912), Vector2D(0, 0), 200, 1, "chest", SDL_Rect{0, 0, 48, 48});
                assets->CreateItem(Vector2D(576, 912), Vector2D(0, 0), 200, 1, "chest", SDL_Rect{0, 0, 48, 48});
                assets->CreateItem(Vector2D(576, 1200), Vector2D(0, 0), 200, 1, "chest", SDL_Rect{0, 0, 48, 48});
                assets->CreateItem(Vector2D(336, 1200), Vector2D(0, 0), 200, 1, "chest", SDL_Rect{0, 0, 48, 48});
                boss->getComponent<TheBosses>().health -= 3;
                break;
            default:
                break;
        }

            // Va cham dan
        for (auto ef : effects) {
            if (Collision::AAABB(bossCol, ef->getComponent<ColliderComponent>().collider))
            {
                boss->getComponent<TheBosses>().health -= 10;
                boss->getComponent<TheBosses>().hit = true;
                Mix_PlayChannel(-1, hitSound, 0);
            }
        }
        for (auto& ss : sssses) {
            if (Collision::AAABB(bossCol, ss->getComponent<ColliderComponent>().collider))
            {
                ss->destroy();
                boss->getComponent<TheBosses>().health -= 10;
                boss->getComponent<TheBosses>().hit = true;
            } else {
                boss->getComponent<TheBosses>().hit = false;
            }
        }
    }

    // Xử lí lớp đạn
    for (auto& p : projectiles) {
        if (Collision::AAABB(playerCol, p->getComponent<ColliderComponent>().collider))
        {
            players[0]->getComponent<KeyboardControler>().health -= 5;
            players[0]->getComponent<KeyboardControler>().hit = true;
            Mix_PlayChannel(-1, main_hit, 0);
            players[0]->getComponent<TransformComponent>().velocity.x = - players[0]->getComponent<TransformComponent>().velocity.x;
            players[0]->getComponent<TransformComponent>().velocity.y = - players[0]->getComponent<TransformComponent>().velocity.y;
            players[0]->getComponent<TransformComponent>().position = playerPos;
            std::cout << "Player health: " << players[0]->getComponent<KeyboardControler>().health << std::endl;
            p->destroy();
        } else {
            players[0]->getComponent<KeyboardControler>().hit = false;
        }
    }

    for (auto& s: skills) {
        SDL_Rect sCol = s->getComponent<ColliderComponent>().collider;
        if (Collision::AAABB(playerCol, sCol))
        {
            players[0]->getComponent<KeyboardControler>().health -= 5;
            players[0]->getComponent<KeyboardControler>().hit = true;
            players[0]->getComponent<TransformComponent>().velocity.x = - players[0]->getComponent<TransformComponent>().velocity.x;
            players[0]->getComponent<TransformComponent>().velocity.y = - players[0]->getComponent<TransformComponent>().velocity.y;
            players[0]->getComponent<TransformComponent>().position = playerPos;
            std::cout << "Player health: " << players[0]->getComponent<KeyboardControler>().health << std::endl;
            s->destroy();
        } else {
            players[0]->getComponent<KeyboardControler>().hit = false;
        }
    }

    // Xu ly lop item
    for (auto& i : items) {
        if (Collision::AAABB(playerCol, i->getComponent<ColliderComponent>().collider))
        {
            players[0]->getComponent<KeyboardControler>().health += 10;
            i->destroy();
        }
    }

    // Xử lý kĩ năng của nhân vật
    flip = players[0]->getComponent<SpriteComponent>().spriteFlip;
    bool isAttack = players[0]->getComponent<KeyboardControler>().isAttacking;
    bool isSkillofPlayer = players[0]->getComponent<KeyboardControler>().isSkill;
    bool isBlocking = players[0]->getComponent<KeyboardControler>().isBlocking;
    Vector2D skillRect;
    if (SDL_GetTicks() - players[0]->getComponent<KeyboardControler>().attack_cd >= 25 && isAttack)
    {
        {
            if (flip == SDL_FLIP_HORIZONTAL) 
                skillRect = playerPos - Vector2D(57,17);
            else 
                skillRect = playerPos + Vector2D(57,-17);

            assets->CreateEffect(playerPos, Vector2D(0, 0), 200, 1, "effect", flip, SDL_Rect{0, 0, 18, 30}, 1);
            Mix_PlayChannel(-1, attackSound, 0);
            players[0]->getComponent<KeyboardControler>().attack_cd = SDL_GetTicks();
        }
    } else {
        isAttack = false;
        for (auto& ef : effects) {
            ef->destroy();
        }
    }

    Vector2D BulletVel = players[0]->getComponent<KeyboardControler>().BulletVel;
    if (SDL_GetTicks() - players[0]->getComponent<KeyboardControler>().skill_cd >= 200 && isSkillofPlayer)
    {
        if (isSkillofPlayer) {
            if (flip == SDL_FLIP_HORIZONTAL) 
                skillRect = playerPos - Vector2D(27,0);
            else 
                skillRect = playerPos + Vector2D(27,0);

            assets->CreateSkillofPlayer(playerPos, BulletVel, 200, 1, "skill2", flip, SDL_Rect{0, 0, 15, 15});
            players[0]->getComponent<KeyboardControler>().skill_cd = SDL_GetTicks();
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

    for (auto& t : tiles) {
        t->Draw();
    }

    for (auto& e : enemies)
    {
        e->Draw();
    }

    for (auto& b : bosses) {
        b->Draw();
    }

    for (auto& p : players) {
        p->Draw();
    }

    for (auto& p : projectiles) {
        p->Draw();
    }

    for (auto& e : effects) {
        e->Draw();
    }

    for (auto& s : skills) {
        s->Draw();
    }

    for (auto& ssss : sssses) {
        ssss->Draw();
    }

    for (auto& i : items) {
        i->Draw();
    }

    CrateHpBar();

    label.Draw();

    SDL_RenderPresent(renderer);
}

void Game::Clean() {
    Mix_FreeChunk(hitSound);
    Mix_FreeChunk(attackSound);
    Mix_FreeChunk(main_hit);
    Mix_FreeChunk(enemy_hit);
    Mix_FreeMusic(bgm);
    hitSound = nullptr;
    attackSound = nullptr;
    main_hit = nullptr;
    enemy_hit = nullptr;
    bgm = nullptr;
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    Mix_Quit();
    SDL_Quit();
    std::cout << "Game cleaned!" << std::endl;
}
