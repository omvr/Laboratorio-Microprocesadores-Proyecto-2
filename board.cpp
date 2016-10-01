#include "Board.h"

Board::Board(SDL_Renderer* renderer): Entity(renderer) {
    SDL_Surface* surface = IMG_Load("red bricks-hi.png");//bloques
    bricktexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("brickwork_in_stretching_bond1000px74perc_svg.png");//paredes del juego
    sidetexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    srand(time(0));
//tamaños y ubicacion de los bloques en la ventana
    x = 16;
    y = 0;
    width = 768;
    height = 600;

    brickoffsetx = 0;
    brickoffsety = 16;
}

Board::~Board() {
    // Clean resources
    SDL_DestroyTexture(bricktexture);
    SDL_DestroyTexture(sidetexture);
}

void Board::Update(float delta) {

}

void Board::Render(float delta) {
    for (int i=0; i<BOARD_WIDTH; i++) {
        for (int j=0; j<BOARD_HEIGHT; j++) {
            Brick brick = bricks[i][j];

            //veo si hay bloques
            if (!brick.state)
                continue;

            //DEfine un area rectangular
            SDL_Rect srcrect;

            //srcrect.x = (brick.type / 2) * BOARD_BRWIDTH;
            //srcrect.y = (brick.type / 2) * BOARD_BRHEIGHT;
            srcrect.x = (1/ 2) * BOARD_BRWIDTH; //hago rectangulos con la imagen de bloques elegida
            srcrect.y = (1/ 2) * BOARD_BRHEIGHT;
            srcrect.w = BOARD_BRWIDTH;
            srcrect.h = BOARD_BRHEIGHT;

            SDL_Rect dstrect;
            dstrect.x = brickoffsetx + x + i * BOARD_BRWIDTH;
            dstrect.y = brickoffsety + y + j * BOARD_BRHEIGHT;
            dstrect.w = BOARD_BRWIDTH;
            dstrect.h = BOARD_BRHEIGHT;

            SDL_RenderCopy(renderer, bricktexture, &srcrect, &dstrect);
        }
    }

    // crear las barras que van a los lados de la ventana
    SDL_Rect dstrect;
    dstrect.x = 0;
    dstrect.y = 0;
    dstrect.w = 16;
    dstrect.h = 600;
    SDL_RenderCopy(renderer, sidetexture, 0, &dstrect);

    dstrect.x = 800 - 16;
    dstrect.y = 0;
    dstrect.w = 16;
    dstrect.h = 600;
    SDL_RenderCopy(renderer, sidetexture, 0, &dstrect);
}

void Board::CreateLevel()//creo los bloques en la ventana
{
    for (int i=0; i<BOARD_WIDTH; i++) {
        for (int j=0; j<BOARD_HEIGHT; j++) {

            Brick brick;
            brick.state = true;
            bricks[i][j] = brick;
        }
    }
}