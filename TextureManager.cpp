//Dùng để load ảnh từ file và đẩy lên texture

#include "TextureManager.h"

TextureManager::TextureManager() {}

TextureManager::~TextureManager() {}

SDL_Texture* TextureManager::LoadTexture(const char* fileName) {
    SDL_Surface* tempSurface = IMG_Load(fileName);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return texture;
}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip) {
    SDL_RenderCopyEx(Game::renderer, texture, &src, &dest, NULL, NULL, flip);
}
