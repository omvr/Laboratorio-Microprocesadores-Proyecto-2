#ifndef BOARD_H_
#define BOARD_H_

#include "Entity.h"
#include <stdlib.h>
#include <time.h>

// Define the dimensions of the board and bricks
// Se Define el tamaño del board y los cuadros
#define BOARD_WIDTH 13
#define BOARD_HEIGHT 5
#define BOARD_BRWIDTH 60
#define BOARD_BRHEIGHT 30

class Brick {
public:
    //int type;
    bool state;
};

class Board: public Entity {
public:
    Board(SDL_Renderer* renderer);
    ~Board();

    void Update(float delta);
    void Render(float delta);
    void CreateLevel();

    float brickoffsetx, brickoffsety;

    // Define the two-dimensional array of bricks
    Brick bricks[BOARD_WIDTH][BOARD_HEIGHT];

private:
    SDL_Texture* bricktexture;
    SDL_Texture* sidetexture;
};

#endif