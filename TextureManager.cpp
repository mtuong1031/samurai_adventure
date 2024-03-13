//Dùng để load ảnh từ file và đẩy lên texture

#include "TextureManager.h"
#include "CommonFunc.h"

TextureManager::TextureManager() 
{
    texture = NULL;
    rect.w = 0;
    rect.h = 0;
}

TextureManager::~TextureManager() 
{
    free();
}

bool TextureManager::LoadTexture(const char *fileName, SDL_Renderer* ren) // Load hinh anh tu duong dan path
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

void TextureManager::RenderTexture(SDL_Renderer* tex, const SDL_Rect* clip) {
    SDL_Rect renderquad = {rect.x, rect.y, rect.w, rect.h}; // Khoi tao vi tri, kich thuoc cua hinh anh
    SDL_RenderCopy(tex, texture, clip, &renderquad); //day thong so cua texture len des
}

void TextureManager::free() {
    if (texture != NULL) {
        SDL_DestroyTexture(texture);
        texture = NULL;
        rect.w = 0;
        rect.h = 0;
    }
}