#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "ECS/ECS.h"
#include "Vector2D.h"

class AssetManager 
{
    public:
        AssetManager(Manager* man);
        ~AssetManager();

        //Quản lí game object
        void CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id);
        void CreateEffect(Vector2D pos, Vector2D vel, int range, int speed, std::string id, SDL_RendererFlip flip, SDL_Rect size);
        void CreateSkill(Vector2D pos, Vector2D vel, int range, int speed, std::string id, SDL_RendererFlip flip, SDL_Rect size);
        void CreatePlayer(Vector2D pos, int speed, std::string id);
        void CreateEnemies(Vector2D pos, Vector2D vel, int range, int speed, std::string id);
        void CreateBoss(Vector2D pos, Vector2D vel, int range, int speed, std::string id, SDL_Rect size);
        //Quản lý texture
        void AddTexture(std::string id, const char* path);
        SDL_Texture* GetTexture(std::string id);

    private:
        Manager* manager;
        std::map<std::string, SDL_Texture*> textures;
};
