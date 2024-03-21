
#include "CommonFunc.h"
#include "TextureManager.h"
#include "character.h"

Map map;
TextureManager background;    
Samurai samurai;
int Samurai::status = 1;

bool InitData() {  
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0) {
        return false;
    } 

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    window = SDL_CreateWindow( "Samurai_Adventure", SDL_WINDOWPOS_UNDEFINED,
                                 SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 
                                 SDL_WINDOW_ALLOW_HIGHDPI ); // Tao cua so game
 
    if (window == NULL) {
        success = false;
    }
     else 
    {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == NULL) {
            success = false;
        } else {
            	
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags)) {
                success = false; 
            }
        }
    }
 
    return success;
}

bool loadIMG() {
    bool retb = background.LoadTexture("bg1.png", renderer);
    bool rets = samurai.LoadCharacter("image/stand_right.png", renderer);
    if (rets == false || retb == false) {
        return false;
    }
    return true;
}

void close() {
    samurai.free();
    background.free();
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    IMG_Quit();
    SDL_Quit();
}


int main(int argc, char *argv[] ) 
{

    SDL_Surface *tempSurface = IMG_Load("bg1.png");
    SDL_Texture *screen = SDL_CreateTextureFromSurface(renderer, tempSurface);

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;  
    int frameTime;

    SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	
    if (InitData() == false) {
        return -1;
    }

    if (loadIMG() == false) {
        return -1;
    }

    map.LoadMap("map_tiles_1.txt");
    map.GetMapTiles(renderer);

    SDL_Event event;
    bool is_quit = false;
    while (!is_quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_quit = true;
            }
            samurai.handleEvent(event); 
        }

        if (samurai.status == Samurai::ATTACK_R || samurai.status == Samurai::ATTACK_L) {
            samurai.setAttack();
        } else {
            samurai.setframe();
        }
        samurai.move();

        camera.x = (samurai.getPosX() + Samurai::samuraiWidth / 2) - SCREEN_WIDTH / 2;
        camera.y = (samurai.getPosY() + Samurai::samuraiHeight / 2) - SCREEN_HEIGHT / 2;

        if (camera.x < 0) {
            camera.x = 0;
        }
        if (camera.y < 0) {
            camera.y = 0;
        }
        if (camera.x > SCREEN_W_LEVEL - camera.w) {
            camera.x = SCREEN_W_LEVEL - camera.w;
        }
        if (camera.y > SCREEN_H_LEVEL - camera.h) {
            camera.y = SCREEN_H_LEVEL - camera.h;
        }
        
        SDL_SetRenderDrawColor(renderer, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(renderer);

        background.RenderTexture(renderer, &camera);         
        map.DrawMap(renderer);             

	    frameStart = SDL_GetTicks();

        samurai.render(renderer, camera.x, camera.y);   

        SDL_RenderPresent(renderer);    // Cap nhat man hinh

	    frameTime = SDL_GetTicks() - frameStart; // Thời gian mất để render 1 frame 

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime); // Nếu thời gian mất để render 1 frame nhỏ hơn thời gian render 1 frame thì delay , tránh bị chậm.
        }
	
    }

    return 0;
}



