#include "character.h"
#include "TextureManager.h"
#include "CommonFunc.h"

Samurai::Samurai() {
    mPosX = SCREEN_WIDTH/2;
    mPosY = SCREEN_WIDTH/2;

    mVelX = 0;
    mVelY = 0;

    rect.w = samuraiWidth;   
    rect.h = samuraiHeight;

    frame = 0;
    status = 0;
}

Samurai::~Samurai() {}

bool Samurai::LoadCharacter(const char *fileName, SDL_Renderer* ren) // Load hinh anh tu duong dan path
{
    SDL_Texture* new_texture = NULL; // Bien tam de load hinh anh

    SDL_Surface* tmpsurface = IMG_Load(fileName); // Load hinh anh tu duong dan path

    if (tmpsurface != NULL) 
    {
        SDL_SetColorKey(tmpsurface, SDL_TRUE, SDL_MapRGB(tmpsurface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B)); // Thiet lap mau trong suot cho hinh anh, nham xoa phan vien cho nv(chi so mau vien = chi so thiet lap)
        new_texture = SDL_CreateTextureFromSurface(ren, tmpsurface); // Tao texture tu surface
        if (new_texture != NULL) // new_texture luu tru thong tin cua tam' anh w, h
        {
            rect.w = tmpsurface->w;   
            rect.h = tmpsurface->h; 
        }
        SDL_FreeSurface(tmpsurface); // Giai phong bo nho cua load_surface vi thong tin anh da dc luu trong new_texture
    }

    texture = new_texture;
    return texture != NULL;
}

void Samurai::setframe() {
    for (int i = 0; i < 6; i++) {
        frameClips[i].x = i * 60;
        frameClips[i].y = 0;
        frameClips[i].w = 60;
        frameClips[i].h = 60;
    }  

}

void Samurai::render(SDL_Renderer* renderer) {

    switch (status)
    {
        case RIGHT:
            LoadCharacter("image/samurai_right.png", renderer);
            break;
        case LEFT:
            LoadCharacter("image/samurai_left.png", renderer);
            break;
        case STAND_R:
            LoadCharacter("image/samurai_stand.png", renderer);
            break;
        case STAND_L:
            LoadCharacter("image/samurai_stand - left.png", renderer);
            break;
    }

    if (status == 0) {
        frame = 0;
    } else {
        frame++;
        if (frame >= 6) {
            frame = 0;
        }   
    }

    rect.x = mPosX;
    rect.y = mPosY;

    SDL_Rect* currentClip = &frameClips[frame];

    SDL_Rect renderQuad = {rect.x, rect.y, samuraiWidth, samuraiHeight};

    SDL_RenderCopy(renderer, texture, currentClip, &renderQuad);
}


void Samurai::handleEvent(SDL_Event &e) {
    // Nếu có sự kiện bàn phím
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0) 
    {
        // Điều khiển samurai, di chuyển theo hướng phím được nhấn
        switch (e.key.keysym.sym) {
            case SDLK_w:
                mVelY -= samuraiSpeed; 
                status = UP;  break;
            case SDLK_s:
                 mVelY += samuraiSpeed; 
                status = DOWN;  break;
            case SDLK_a:
                mVelX -= samuraiSpeed;
                status = LEFT;  break;
            case SDLK_d:
                mVelX += samuraiSpeed;
                status = RIGHT;  break;
        }
    }
    else if (e.type == SDL_KEYUP && e.key.repeat == 0) 
    {
        // Điều khiển samurai, ngung di chuyển khi tha phim
        switch (e.key.keysym.sym) {
            case SDLK_w:
                mVelY += samuraiSpeed; 
                status = 0;  break;
            case SDLK_s:
                mVelY -= samuraiSpeed; 
                status = 0;  break;
            case SDLK_a:
                mVelX += samuraiSpeed;
                status = STAND_L;  break;
            case SDLK_d:
                mVelX -= samuraiSpeed;
                status = STAND_R;  break;
        }
    }
}

void Samurai::move() {
    // Di chuyển samurai theo vận tốc
    mPosX += mVelX;
    rect.x = mPosX;

    // Nếu samurai di chuyển ra khỏi màn hình
    if((mPosX < 0) || (mPosX + samuraiWidth > SCREEN_WIDTH) || checkMapCollision(map)) {
        // Di chuyển lại
        mPosX -= mVelX;
        rect.x = mPosX;
    }

    mPosY += mVelY;
    rect.y = mPosY;  

    // Nếu samurai di chuyển ra khỏi màn hình
    if ((mPosY < 0) || (mPosY + samuraiHeight > SCREEN_HEIGHT) || checkMapCollision(map) ) {
        // Di chuyển lại
        mPosY -= mVelY;
        rect.y = mPosY;
    }
}

bool Samurai::checkMapCollision(Map &map) {
    int x1, x2, y1, y2;
    x1 = mPosX / 64;
    x2 = (mPosX + samuraiWidth) / 64;
    y1 = mPosY / 64;
    y2 = (mPosY + samuraiHeight) / 64;

    map.LoadMap("tilemap.txt");
 // Kiểm tra va chạm với map
    if (x1 >= 0 && x2 < 30 && y1 >= 0 && y2 < 20) {
        if (map.getMapValue(x1, y1) != 0 || map.getMapValue(x2, y1) != 0 || map.getMapValue(x1, y2) != 0 || map.getMapValue(x2, y2) != 0) { 
            return true;
        }
    }   
    return false;
}

void Samurai::free() {
    if (texture != NULL) {
        SDL_DestroyTexture(texture);
        texture = NULL;
        rect.w = 0;
        rect.h = 0;
    }
}