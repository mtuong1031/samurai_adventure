#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "ECS/ECS.h"
#include "Vector2D.h"
#include <SDL2/SDL_ttf.h>

class AssetManager 
{
    public:
        AssetManager(Manager* man);
        ~AssetManager();

        //Quản lí game object
        void CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id);
        void CreateEffect(Vector2D pos, Vector2D vel, int range, int speed, std::string id, SDL_RendererFlip flip, SDL_Rect size, int index);
        void CreateSkillofPlayer(Vector2D pos, Vector2D vel, int range, int speed, std::string id, SDL_RendererFlip flip, SDL_Rect size);
        void CreateSkill(Vector2D pos, Vector2D vel, int range, int speed, std::string id, SDL_RendererFlip flip, SDL_Rect size, int index);
        void CreatePlayer(Vector2D pos, int speed, std::string id);
        void CreateEnemies(Vector2D pos, Vector2D vel, int range, int speed, std::string id);
        void CreateBoss(Vector2D pos, Vector2D vel, int range, int speed, std::string id, SDL_Rect size);
        void CreateItem(Vector2D pos, Vector2D vel, int range, int speed, std::string id, SDL_Rect size);
        //Quản lý texture
        void AddTexture(std::string id, const char* path);
        SDL_Texture* GetTexture(std::string id);

        void AddFont(std::string id, std::string path, int fontSize);
        TTF_Font* GetFont(std::string id);

        // void AddHpBar(std::string id, const char* path);

    private:
        Manager* manager;
        std::map<std::string, SDL_Texture*> textures;
        std::map<std::string, TTF_Font*> fonts;
};
