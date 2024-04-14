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
        void CreatePlayer(Vector2D pos, int speed, std::string id);
        void CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id);
        void CreateEnemy(Vector2D pos, int range, int speed, std::string id);

        //Quản lý texture
        void AddTexture(std::string id, const char* path);
        SDL_Texture* GetTexture(std::string id);

    private:
        Manager* manager;
        std::map<std::string, SDL_Texture*> textures;
};
