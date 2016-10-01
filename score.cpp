#include "score.h"
#include "SDL2/SDL_ttf.h"
#include "stdlib.h"

#include <string>
#include <iostream>
#include <sstream>
#include <stdlib.h>

score::score(SDL_Renderer* renderer): Entity(renderer) {

    Sans2 = TTF_OpenFont("FreeSans.ttf", 18);

}

score::~score() {
    // Clean resources
    SDL_DestroyTexture(ScoreMessage);
    //SDL_FreeSurface(surfaceScore2);
    //SDL_FreeSurface(surfaceScore);
}


void score::Stadist(int scores, int lives, bool press_x){//int scores,int lives) {

    //std::string score_text = "score: " + std::to_string(2);

    //int number=3;
    std::stringstream str_scores;
    std::stringstream str_lives;
    str_scores << scores;
    str_lives << lives;

    std::string temp_scores = "Score: " + str_scores.str() + "   Lives: " + str_lives.str();
    char* text_score = (char*)  temp_scores.c_str();

    SDL_Color White = {255, 255, 255};

    if (press_x){

                SDL_Surface *surface_press_x = TTF_RenderText_Solid(Sans2, "Press X", White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

                SDL_Texture *press_x_Message = SDL_CreateTextureFromSurface(renderer, surface_press_x); //now you can convert it into a texture
                SDL_FreeSurface(surface_press_x);

                SDL_Rect press_x_rect; //create a rect
                press_x_rect.x = 300; //controls the rect's x coordinate
                press_x_rect.y = 300; // controls the rect's y coordinte
                press_x_rect.w = 200; // controls the width of the rect
                press_x_rect.h = 64; // controls the height of the rect
                SDL_RenderCopy(renderer, press_x_Message, NULL,&press_x_rect);
                SDL_DestroyTexture(press_x_Message);
            }

    surfaceScore = TTF_RenderText_Solid(Sans2, text_score, White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first


    ScoreMessage = SDL_CreateTextureFromSurface(renderer, surfaceScore); //now you can convert it into a texture
    SDL_FreeSurface(surfaceScore);

    SDL_Rect score_rect; //create a rect
    score_rect.x = 20; //controls the rect's x coordinate
    score_rect.y = 530; // controls the rect's y coordinte
    score_rect.w = 200; // controls the width of the rect
    score_rect.h = 64; // controls the height of the rect
    SDL_RenderCopy(renderer, ScoreMessage, NULL,&score_rect);
}