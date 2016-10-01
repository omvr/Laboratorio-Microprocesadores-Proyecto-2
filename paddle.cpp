#include "Paddle.h"
//#include "SDL2/SDL_ttf.h"

Paddle::Paddle(SDL_Renderer* renderer): Entity(renderer) {
    SDL_Surface* surface = IMG_Load("Nube_Voladora_de_Amatista.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);


    SDL_FreeSurface(surface);
    //tamaño y posicion de la plataforma
    width = 128;
    height = 32;

    y = 530; //posicion para poder poner las vidas y el score debajo de la plataforma
}

Paddle::~Paddle() {
    // Clean resources
    SDL_DestroyTexture(texture);
}


void Paddle::Update(float delta) {

}

void Paddle::Render(float delta) {
    SDL_Rect rect;
    rect.x = (int)(x + 0.5f);
    rect.y = (int)(y + 0.5f);
    rect.w = width;
    rect.h = height;
    SDL_RenderCopy(renderer, texture, 0, &rect);
}