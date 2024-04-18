#include "AssetManager.h"
#include "ECS/Component.h"

AssetManager::AssetManager(Manager* man) : manager(man) 
{    }


AssetManager::~AssetManager() {    }

void AssetManager::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id) {
    auto& projectile(manager->addEntity());

    projectile.addComponent<TransformComponent>(pos.x, pos.y, 10, 10, 2);
    projectile.addComponent<SpriteComponent>(id, true, 2, 200, 10, 10, 10 , 10);
    projectile.addComponent<ProjectileComponent>(range, speed, vel, id);
    projectile.addComponent<ColliderComponent>("projectile", pos.x, pos.y, 10, 10);
    projectile.addGroup(Game::groupProjectiles);
}

void AssetManager::CreateEffect(Vector2D pos, Vector2D vel, int range, int speed, std::string id, SDL_RendererFlip flip, SDL_Rect size, int index) {
    auto& effect(manager->addEntity());

    effect.addComponent<TransformComponent>(pos.x, pos.y, size.h, size.w, 3);
    effect.addComponent<SpriteComponent>(id, true, 4, 100, size.w, size.h, flip);
    effect.addComponent<ProjectileComponent>(range, speed, vel, id, index);
    effect.addComponent<ColliderComponent>("effect");
    effect.addGroup(Game::groupEffects);
}

void AssetManager::CreateSkill(Vector2D pos, Vector2D vel, int range, int speed, std::string id, SDL_RendererFlip flip, SDL_Rect size) {
    auto& skill(manager->addEntity());

    skill.addComponent<TransformComponent>(pos.x, pos.y, size.h, size.w, 2);
    skill.addComponent<SpriteComponent>(id, true, 4, 100, size.w, size.h, flip);
    skill.addComponent<ProjectileComponent>(range, speed, vel, id);
    skill.addComponent<ColliderComponent>("skill", pos.x, pos.y, size.w, size.h);
    skill.addGroup(Game::groupSkills);
}

void AssetManager::CreatePlayer(Vector2D pos, int speed, std::string id) {
    auto& player(manager->addEntity());
    Vector2D bulletVel(0, 0);
    player.addComponent<TransformComponent>(pos.x, pos.y, 19, 19, 3);
    player.addComponent<SpriteComponent>(id, true);
    player.addComponent<KeyboardControler>();
    player.addComponent<ColliderComponent>("player", pos.x, pos.y, 57);

    // bulletVel.x = player.getComponent<KeyboardControler>().BulletVel.x;
    // bulletVel.y = player.getComponent<KeyboardControler>().BulletVel.y;
    bulletVel = player.getComponent<KeyboardControler>().BulletVel;
    AssetManager::CreateProjectile(pos, bulletVel, 200, 2, "hp");

    player.addGroup(Game::groupPlayers);
}

void AssetManager::CreateEnemies(Vector2D pos, Vector2D vel, int range, int speed, std::string id) {
    auto& enemy(manager->addEntity());

    enemy.addComponent<TransformComponent>(pos.x, pos.y, 64, 64, 1);
    enemy.addComponent<SpriteComponent>(id, true, 12, 200, 64, 64, 160, 64);
    enemy.addComponent<TheEnemies>(pos, range, speed, vel);
    enemy.addComponent<ColliderComponent>("enemy");
    enemy.addGroup(Game::groupEnemies);
}

void AssetManager::CreateBoss(Vector2D pos, Vector2D vel, int range, int speed, std::string id, SDL_Rect size) 
{
    auto& boss(manager->addEntity());

    boss.addComponent<TransformComponent>(pos.x, pos.y, size.h, size.w, 2);
    boss.addComponent<SpriteComponent>(id, true, 8, 200, size.w, size.h, size.w, size.h);
    boss.addComponent<TheEnemies>(pos, range, speed, vel, 1000);
    boss.addComponent<ColliderComponent>("boss");
    boss.addGroup(Game::groupBosses);

}

void AssetManager::AddTexture(std::string id, const char* path) {
    textures.emplace(id, TextureManager::LoadTexture(path));
}


SDL_Texture* AssetManager::GetTexture(std::string id) {
    return textures[id];
}