#include "AssetManager.h"
#include "ECS/Component.h"

AssetManager::AssetManager(Manager* man) : manager(man) 
{    }


AssetManager::~AssetManager() {    }

void AssetManager::CreatePlayer(Vector2D pos, int speed, std::string id) {
    auto& player(manager->addEntity());

    player.addComponent<TransformComponent>(pos.x, pos.y, 80, 60, 1);
    player.addComponent<SpriteComponent>(id, true);
    player.addComponent<KeyboardControler>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(Game::groupPlayers);
}

void AssetManager::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id) {
    auto& projectile(manager->addEntity());

    projectile.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 1);
    projectile.addComponent<SpriteComponent>(id, false);
    projectile.addComponent<ProjectileComponent>(range, speed, vel);
    projectile.addComponent<ColliderComponent>("projectile");
    projectile.addGroup(Game::groupProjectiles);
}

void AssetManager::CreateEnemies(Vector2D pos, Vector2D vel, int range, int speed, std::string id) {
    auto& enemy(manager->addEntity());

    enemy.addComponent<TransformComponent>(pos.x, pos.y, 64, 64, 1);
    enemy.addComponent<SpriteComponent>(id, true, 12, 150, 64, 64, 160, 64);
    enemy.addComponent<TheEnemies>(pos, range, speed, vel);
    enemy.addComponent<ColliderComponent>("enemy");
    enemy.addGroup(Game::groupEnemies);
}

void AssetManager::AddTexture(std::string id, const char* path) {
    textures.emplace(id, TextureManager::LoadTexture(path));
}


SDL_Texture* AssetManager::GetTexture(std::string id) {
    return textures[id];
}