#define SAMURAI_H_
#define SAMURAI_H_

#include "CommonFunc.h"
#include "TextureManager.h"
#include "Map.h"

class Samurai{
    public:
        Samurai(); 
        ~Samurai();
        static const int samuraiWidth = 60;
        static const int samuraiHeight = 60;

        static const int samuraiSpeed = 10;

        enum ANIMATION_STATUS {STAND_R = 6, RIGHT = 1, LEFT = 2, UP = 3, DOWN = 4, ATTACK = 5, STAND_L = 7}; 

        bool LoadCharacter(const char *fileName, SDL_Renderer* ren);
        void render(SDL_Renderer* renderer);
        void handleEvent(SDL_Event &e);
        void setframe();
        bool checkMapCollision(Map &map);
        void move();
        void free();

    private:
        int mPosX, mPosY;  

        int mVelX, mVelY; // Vận tốc của samurai

        SDL_Rect rect; 

        Map map;

        SDL_Texture* texture; 

        SDL_Rect frameClips[6]; // Mảng chứa các clip của samurai
        int frame;
        int status;
};


#ifdef SAMURAI_H_
#endif