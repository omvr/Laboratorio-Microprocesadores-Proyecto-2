#include "Ball.h"

Ball::Ball(SDL_Renderer* renderer): Entity(renderer) {
    SDL_Surface* surface = IMG_Load("Ball_3.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    //posicion inicial de la bola y tamaño
    x = 0;
    y = 0;
    width = 24;
    height = 24;

    SetDirection(1, 1);
}

Ball::~Ball() {
    // Clean resources
    SDL_DestroyTexture(texture);
}
//mantengo la actualizacion de la direccion de la bola
void Ball::Update(float delta) {
    x += dirx * delta;
    y += diry * delta;
}
//mantego recreada la bola
void Ball::Render(float delta) {
    SDL_Rect rect;
    rect.x = (int)(x + 0.5f);
    rect.y = (int)(y + 0.5f);
    rect.w = width;
    rect.h = height;
    SDL_RenderCopy(renderer, texture, 0, &rect);
}

void Ball::SetDirection(float dirx, float diry) {
    // Normalize the direction vector and multiply with BALL_SPEED
    float length = sqrt(dirx * dirx + diry * diry);
    this->dirx = BALL_SPEED * (dirx / length);
    this->diry = BALL_SPEED * (diry / length);
}