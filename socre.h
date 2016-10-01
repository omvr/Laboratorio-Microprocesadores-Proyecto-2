#ifndef SCORE_H_
#define SCORE_H_

#include "Entity.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL.h"
//#include "stdio.h"
#include <string>
#include <sstream>

class score: public Entity {
public:
    int scores;
    int lives;
    SDL_Rect Message_rect;
    SDL_Texture* text;
    TTF_Font* Sans2;
    SDL_Color White;
    SDL_Surface* surfaceScore;
    SDL_Texture* ScoreMessage;

    std::ostringstream ss;

    score(SDL_Renderer* renderer);
    void Stadist(int scores, int lives,bool press_x);//int scores,int lives);
    ~score();

private:


};

#endif