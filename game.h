#ifndef GAME_H_
#define GAME_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <stdio.h>
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"

#include "Board.h"
#include "Paddle.h"
#include "Ball.h"
#include "score.h"



#define FPS_DELAY 500

class Game {
public:
    //The music that will be played
    Mix_Music *music = NULL;
    //The sound effects that will be used
    Mix_Chunk *bounce = NULL;
    int scores;
    int lives;
    bool playing;
    bool press_x;
    bool reset;

    Game();
    ~Game();


    bool Init();
    void Bienvenida();
    void Run();
    void Salida();
    void ShowScore();
    void GetCPUinfo();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    //SDL_Texture* texture;

    // Timing
    unsigned int lasttick, fpstick, fps, framecount;

    // Test
    float testx, testy;

    Board* board;
    Paddle* paddle;
    Ball* ball;
    score* Score;
    bool paddlestick;

    void Clean();
    void Stadist();//int scores, int lives);
    void Update(float delta);
    void Render(float delta,int scores, int lives);


    void NewGame();
    void ResetPaddle();
    void StickBall();

    void SetPaddleX(float x);
    void CheckBoardCollisions();
    float GetReflection(float hitx);
    void CheckPaddleCollisions();
    void CheckBrickCollisions();
    void CheckBrickCollisions2();
    void BallBrickResponse(int dirindex);
    int GetBrickCount();
};

#endif