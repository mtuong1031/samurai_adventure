
#include "CommonFunc.h"
#include "TextureManager.h"
#include "character.h"
TextureManager background;    
 
 Samurai samurai;

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
    bool retb = background.LoadTexture("dune.png", renderer);
    bool rets = samurai.LoadCharacter("image/samurai_stand.png", renderer);
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

    SDL_Surface *tempSurface = IMG_Load("dune.png");
    SDL_Texture *screen = SDL_CreateTextureFromSurface(renderer, tempSurface);

    const int FPS = 12;
    const int frameDelay = 1000 / FPS;

    Uint32 frameStart;  
    int frameTime;

	
    if (InitData() == false) {
        return -1;
    }

    if (loadIMG() == false) {
        return -1;
    }

    samurai.setframe();
    samurai.setframe();

    SDL_Event event;
    bool is_quit = false;
    while (!is_quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                is_quit = true;
            }
            samurai.handleEvent(event);

        }

        SDL_SetRenderDrawColor(renderer, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(renderer);
        background.RenderTexture(renderer, NULL);

	    frameStart = SDL_GetTicks();

        samurai.move();
        samurai.render(renderer);   

        SDL_RenderPresent(renderer);    // Cap nhat man hinh
	

	    frameTime = SDL_GetTicks() - frameStart; // Thời gian mất để render 1 frame 

        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime); // Nếu thời gian mất để render 1 frame nhỏ hơn thời gian render 1 frame thì delay , tránh bị chậm.
        }
	
    }

    return 0;
}