#include "GameObject.h"
#include "TextureManager.h"


GameObject::GameObject(const char* texturesheet,SDL_Renderer *ren, int x, int y) {
    renderer = ren;
    objTexture = TextureManager::LoadTexture(texturesheet, renderer);

    xpos = x;
    ypos = y;
}


void GameObject::Update() {
    xpos++;
    ypos++;
    
    srcRect.h = 64;
    srcRect.w = 64;
    srcRect.x = 0;
    srcRect.y = 0;

    destRect.x = xpos;
    destRect.y = ypos;
    destRect.w = srcRect.w;
    destRect.h = srcRect.h;
}


void GameObject::Render() {
    SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect);
}