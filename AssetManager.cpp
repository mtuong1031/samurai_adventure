#include "AssetManager.h"
#include "ECS/Component.h"

AssetManager::AssetManager(Manager* man) : manager(man) 
{    }


AssetManager::~AssetManager() {    }

void AssetManager::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id) {
    auto& projectile(manager->addEntity());

    projectile.addComponent<TransformComponent>(pos.x, pos.y, 64, 64, 1);
    projectile.addComponent<SpriteComponent>(id, true, 5, 200, 32, 7, 16 , 16);
    projectile.addComponent<ProjectileComponent>(range, speed, vel);
    projectile.addComponent<ColliderComponent>("projectile");
    projectile.addGroup(Game::groupProjectiles);
}

void AssetManager::CreateEffect(Vector2D pos, Vector2D vel, int range, int speed, std::string id, SDL_RendererFlip flip) {
    auto& effect(manager->addEntity());

    effect.addComponent<TransformComponent>(pos.x, pos.y, 30, 18, 4);
    effect.addComponent<SpriteComponent>(id, true, 4, 100, 18, 30, flip);
    effect.addComponent<ProjectileComponent>(range, speed, vel);
    effect.addComponent<ColliderComponent>("effect");
    effect.addGroup(Game::groupEffects);
}


void AssetManager::CreatePlayer(Vector2D pos, int speed, std::string id) {
    auto& player(manager->addEntity());
    Vector2D bulletVel(0, 0);
    player.addComponent<TransformComponent>(pos.x, pos.y, 19, 19, 4);
    player.addComponent<SpriteComponent>(id, true);
    player.addComponent<KeyboardControler>();
    player.addComponent<ColliderComponent>("player", pos.x, pos.y, 78);

    // bulletVel.x = player.getComponent<KeyboardControler>().BulletVel.x;
    // bulletVel.y = player.getComponent<KeyboardControler>().BulletVel.y;
    bulletVel = player.getComponent<KeyboardControler>().BulletVel;
    AssetManager::CreateProjectile(pos, bulletVel, 200, 2, "hp");

    player.addGroup(Game::groupPlayers);
}

void AssetManager::CreateEnemies(Vector2D pos, Vector2D vel, int range, int speed, std::string id) {
    auto& enemy(manager->addEntity());

    enemy.addComponent<TransformComponent>(pos.x, pos.y, 64, 64, 1);
    enemy.addComponent<SpriteComponent>(id, true, 12, 500, 64, 64, 160, 64);
    enemy.addComponent<TheEnemies>(pos, range, speed, vel);
    enemy.addComponent<ColliderComponent>("enemy");
    // enemy.getComponent<TheEnemies>().original_vector = pos;

    // float length = sqrt(pow(Game::playerRect.x - pos.x, 2) + pow(Game::playerRect.y - pos.y, 2));
    // float dx = (Game::playerRect.x - pos.x) / length;
    // float dy = (Game::playerRect.y - pos.y) / length;
    // vel.x = dx;
    // vel.y = dy;

    // AssetManager::CreateProjectile(pos, vel, range, speed, "projectile");
    enemy.addGroup(Game::groupEnemies);
}

void AssetManager::AddTexture(std::string id, const char* path) {
    textures.emplace(id, TextureManager::LoadTexture(path));
}


SDL_Texture* AssetManager::GetTexture(std::string id) {
    return textures[id];
}