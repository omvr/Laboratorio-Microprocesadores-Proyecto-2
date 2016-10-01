#include "Game.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_keyboard.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_ttf.h"

#include <stdio.h>
#include <wiringPi.h>
#include <iostream>

const int Butpin=17;
using namespace std;



Game::Game() {
    wiringPiSetupGpio();
pinMode(Butpin, INPUT);
pullUpDnControl(Butpin, PUD_UP);
    window = 0;
    renderer = 0;
}

Game::~Game() {



}

bool Game::Init() {
    SDL_Init(SDL_INIT_VIDEO); // inicializo el driver de video del sistema

    // Codigo para crear la ventana
    window = SDL_CreateWindow("MICRONOID",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (!window) {
        std::cout << "Error creating window:" << SDL_GetError() << std::endl;
        return false;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE); // cambiar el renderer para la raspberry
    if (!renderer) {
        std::cout << "Error creating renderer:" << SDL_GetError() << std::endl;
        return false;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////


    //Initialize SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ) {
    return false;
    }
    //Set the window caption
    //SDL_WM_SetCaption( "Monitor Music", NULL );
    ///////////////////////////////////////////////////////////////////////////////////////////
    music = Mix_LoadMUS( "music_aud.wav" );
    //If there was a problem loading the music
    if( music == NULL ) { return false; }
    //Load the sound effects
    bounce = Mix_LoadWAV( "bounce_aud.wav" );
    //If there was a problem loading the sound effects
    if( bounce == NULL ) { return false; }
////////////////////////////////////////////

    if (TTF_Init() != 0)
    {
       // cerr << "TTF_Init() Failed: " << TTF_GetError() << endl;
        SDL_Quit();
        exit(1);
    }

    // Initialize resources

    //SDL_Surface* surface = IMG_Load("test.png");
    //texture = SDL_CreateTextureFromSurface(renderer, surface);
    //SDL_FreeSurface(surface);

    // Initialize timing
    lasttick = SDL_GetTicks();
    fpstick = lasttick;
    fps = 0;
    framecount = 0;

    testx = 0;
    testy = 0;

    return true;



}

//Pantalla de bienvenida con una duracion de 5 segundos antes de iniciar el juego
void Game::Bienvenida(){
SDL_Surface *background_surface = IMG_Load("bienvenida.jpg");
        //SDL_FreeSurface(background_surface);
        if(background_surface == NULL)
        {
            SDL_ShowSimpleMessageBox(0, "Background init error",SDL_GetError(), window);
        }

        SDL_Texture *background_texture  = SDL_CreateTextureFromSurface(renderer,background_surface);
        if(background_texture == NULL)
        {
            SDL_ShowSimpleMessageBox(0, "Texture init error", SDL_GetError(), window);
        }
        SDL_RenderCopy(renderer, background_texture, NULL, NULL);
        SDL_RenderPresent(renderer);
         /////////////////////////////////////////////////////////////////////////////////////////////////////////////
         SDL_Delay(5000);

}

//pantalla de salida si se gana, pierde o se sale del juego voluntariamente
void Game::Salida(){

        SDL_Surface *background_surface = IMG_Load("Game_over.png");
        //SDL_FreeSurface(background_surface);
        if(background_surface == NULL)
        {
            SDL_ShowSimpleMessageBox(0, "Background init error",SDL_GetError(), window);
        }

        SDL_Texture *background_texture  = SDL_CreateTextureFromSurface(renderer,background_surface);
        //////////////////////////////////////////////////////////////////////////////////////////////////////

        //////////////////////////////////////////////////////////////////////////////////////////////////////

        if(background_texture == NULL)
        {
            SDL_ShowSimpleMessageBox(0, "Texture init error", SDL_GetError(), window);
        }
        SDL_RenderCopy(renderer, background_texture, NULL, NULL);

        ShowScore();
        GetCPUinfo();

        SDL_RenderPresent(renderer);
         /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SDL_Delay(5000);
        SDL_ShowSimpleMessageBox(0, "Micronoid", "Presione Enter Para Salir", window);
    Clean();
}
//muestra el puntaje del usuario al chocar la bola con los bloques y la plataforma
void Game::ShowScore(){
            TTF_Font* Sans2 = TTF_OpenFont("FreeSans.ttf", 24);
            SDL_Color White = {255, 255, 255};


            std::stringstream str_scores;
            std::stringstream str_lives;
            str_scores << scores;

            std::string temp_scores = "Your Score: " + str_scores.str();
            char* text_score = (char*)  temp_scores.c_str();

            SDL_Surface *surface_SCORE = TTF_RenderText_Solid(Sans2, text_score, White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first


            SDL_Texture *Score_Message = SDL_CreateTextureFromSurface(renderer, surface_SCORE); //now you can convert it into a texture
            SDL_FreeSurface(surface_SCORE);

            SDL_Rect score_rect; //create a rect
            score_rect.x = 20; //controls the rect's x coordinate
            score_rect.y = 400; // controls the rect's y coordinte
            score_rect.w = 200; // controls the width of the rect
            score_rect.h = 64; // controls the height of the rect
            SDL_RenderCopy(renderer, Score_Message, NULL,&score_rect);
            SDL_DestroyTexture(Score_Message);
}

/*void Game::ShowPressX(bool press_x){
        if (press_x){
            TTF_Font* Sans2 = TTF_OpenFont("FreeSans.ttf", 24);
            SDL_Color White = {255, 255, 255};

            SDL_Surface *surface_SCORE = TTF_RenderText_Solid(Sans2, "Press X", White); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first

            SDL_Texture *Score_Message = SDL_CreateTextureFromSurface(renderer, surface_SCORE); //now you can convert it into a texture
            SDL_FreeSurface(surface_SCORE);

            SDL_Rect score_rect; //create a rect
            score_rect.x = 100; //controls the rect's x coordinate
            score_rect.y = 400; // controls the rect's y coordinte
            score_rect.w = 200; // controls the width of the rect
            score_rect.h = 64; // controls the height of the rect
            SDL_RenderCopy(renderer, Score_Message, NULL,&score_rect);
            SDL_DestroyTexture(Score_Message);
            }
}*/


void Game::GetCPUinfo(){}

//Funcion eliminar toda la ventana cuando se sale o termina el juego
void Game::Clean() {
    // Clean resources
    //SDL_DestroyTexture(texture);
    delete board;
    delete paddle;
    delete ball;
    delete Score;

    // Clean renderer and window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    Mix_FreeMusic( music );

    //Quit SDL_mixe
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}
//funcion que inicia la contruccion del juego y sus elementos graficos
void Game::Run() {


    //SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
    Score = new score(renderer);
    board = new Board(renderer);
    paddle = new Paddle(renderer);
    ball = new Ball(renderer);
    lives = 3;

    NewGame();
    //Free the music
    Mix_PlayMusic(music, -1 );

    // Main loop
    playing = true;
    press_x = true;
    while (playing)
    {
        // Handler events
        SDL_Event e;  // se crea el evento que estara en loop
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                break;
            }
        }
        //codigo gracias a http://rembound.com/articles/the-breakout-tutorial
        // Calculate delta and fps
        unsigned int curtick = SDL_GetTicks();
        float delta = (curtick - lasttick) / 1000.0f;
        if (curtick - fpstick >= FPS_DELAY) {
            fps = framecount * (1000.0f / (curtick - fpstick));
            fpstick = curtick;
            framecount = 0;
            //std::cout << "FPS: " << fps << std::endl;
            char buf[100];
            snprintf(buf, 100, "MICRONOID (fps: %u)", fps);
            SDL_SetWindowTitle(window, buf);
        } else {
            framecount++;
        }
        lasttick = curtick;

        // Update and render the game
        //
        Update(delta);
        Render(delta,scores, lives);

    }
    //condicion para mostrar si se gano o perdio
    if (lives == 0){SDL_ShowSimpleMessageBox(0, "Micronoid", "Juego finalizado, Mejor suerte la proxima vez", window);}
    else {SDL_ShowSimpleMessageBox(0, "Micronoid", "Felicidades, Juego Terminado", window);}

}
//reinicia todo los elementos visuales del juego a su estado inicial, los bloques y la plataforma
void Game::NewGame() {

    board->CreateLevel();

    ResetPaddle();

}
//reiniciar posicion de la plataforma y la bola en el juego
void Game::ResetPaddle() {
    paddlestick = true;
    StickBall();
}

//Coloca la pelota en la posicion
void Game::StickBall() {
    ball->x = paddle->x + paddle->width/2 - ball->width/2;
    ball->y = paddle->y - ball->height;
}

//esta es la funcion que mantiene el refresco del juego, en el loop, pasa recreando los elementos y mantiene las colisiones con la ventana
void Game::Update(float delta) {

    /////////////////////////////////////////////////////
    //CODIGO TECLADO                                   //
    /////////////////////////////////////////////////////
    if(digitalRead(Butpin)){
    const Uint8 *keystate;// declaro el puntero sobre el que quiero que este esperando tecla
    if (paddlestick)
            {
                SetPaddleX(board->width/2.0f-BOARD_BRWIDTH);// para ubicar la plataforma en el medio
            }



        keystate=SDL_GetKeyboardState(NULL);
        if(keystate[SDL_SCANCODE_X])
        {
            press_x = false;
          //  printf("PUDE EN KEYSTATE?");//ESTE CODIGO ES EL QUE AVANZA Y OBTIENE LA TECLA
            if (paddlestick)
            {
                paddlestick = false;
                ball->SetDirection(1, -1);
            }
        }

        if(keystate[SDL_SCANCODE_ESCAPE])// termina el juego si se apreta la tecla Esc
        {
              playing = false;
              lives = 0;
        }

         if(keystate[SDL_SCANCODE_LEFT])// movimiento de la platforma, el movimiento es mejor en la raspberry, en la pc es mucho mas rapido
        {
                SetPaddleX(paddle->x - 0.015f*board->width);
              //  SetPaddleX(board->width -  board->width/2.0f);

        }
        if(keystate[SDL_SCANCODE_RIGHT])
        {
                 SetPaddleX(paddle->x + 0.015f*board->width);

              //  SetPaddleX(board->width - board->width/2.0f);

        }


    if (paddlestick)//condicion para mantener la bola en la posicion inicial junto con la plataforma
    {
        StickBall();

    }
    //llamo a las funciones que me mantienen revisando por donde hay choques de la bola con los bloques y las barras de la ventana
    CheckBoardCollisions();
    CheckPaddleCollisions();
    CheckBrickCollisions2();

    if (GetBrickCount() == 0)//se acaba el juego si se acaban los bloques
    {
        playing = false;
    }
    //mantengo actualizado la pantalla y la plataforma
    board->Update(delta);
    paddle->Update(delta);

    if (!paddlestick)//si la plataforma no esta fija actualizo la posicion de la bola
    {
        ball->Update(delta);
    }
    }// fin if boton externo de la raspberry

    else{
            scores=0;
            Bienvenida();
            Run();
        //(0,0,false);
        //NewGame();
       // Clean();
    }

}////fin void GAME
//////////////////////////////////////Input mouse///////////////////////////////////////////


void Game::SetPaddleX(float x) {//posiciono la plataforma
    float newx;
    if (x < board->x) {
        // Upper bound
        newx = board->x;
    } else if (x + paddle->width > board->x + board->width) {
        // Lower bound
        newx = board->x + board->width - paddle->width;
    } else {
        newx = x;
    }
    paddle->x = newx;
}

void Game::CheckBoardCollisions() {
    // Top and bottom collisions
    if (ball->y < board->y) {
        // Top
        // Keep the ball within the board and reflect the y-direction
        Mix_PlayChannel( -1, bounce, 0 );// sonidos de rebote
        ball->y = board->y;
        ball->diry *= -1;
        Mix_PlayChannel( -1, bounce, 0 );// sonidos de rebote
    } else if (ball->y + ball->height > board->y + board->height) {
        // Bottom

        // Ball lost
        ResetPaddle();
        press_x = true;
        //pierdo vidas si hay choque en el fondo de la pantalla
        if (lives == 1){
            lives -=1;
            playing = false;
        } else {lives -=1;}

    }

    // Left and right collisions
    if (ball->x <= board->x) {
        // Left
        // Keep the ball within the board and reflect the x-direction
        Mix_PlayChannel( -1, bounce, 0 );// sonidos de rebote
        ball->x = board->x;
        ball->dirx *= -1;
    } else if (ball->x + ball->width >= board->x + board->width) {
        // Right
        Mix_PlayChannel( -1, bounce, 0 );// sonidos de rebote
        // Keep the ball within the board and reflect the x-direction
        ball->x = board->x + board->width - ball->width;
        ball->dirx *= -1;

    }
}

float Game::GetReflection(float hitx) {
    //Hacer que la bola rebote cuando toca la plataforma para continuar con el juego
    if (hitx < 0) {
        hitx = 0;
    } else if (hitx > paddle->width) {
        hitx = paddle->width;
    }

    // choques con la parte izauierda de la plataforma toman direccion hacia la izquierda y lo mismo para la derecha
    hitx -= paddle->width / 2.0f;

    //mantener la escala del rebote
    return 2.0f * (hitx / (paddle->width / 2.0f));
}


void Game::CheckPaddleCollisions() { //ver donde hay colision con la plataforma
    //centrar la bola
    float ballcenterx = ball->x + ball->width / 2.0f;

    if (ball->Collides(paddle)) {
        ball->y = paddle->y - ball->height;
        ball->SetDirection(GetReflection(ballcenterx - paddle->x), -1);
        Mix_PlayChannel( -1, bounce, 0 );// sonidos de rebote
        scores++;
        //ball->SetDirection(0, -1);
    }
}

void Game::CheckBrickCollisions() {//choques con los bloques
    for (int i=0; i<BOARD_WIDTH; i++) {
        for (int j=0; j<BOARD_HEIGHT; j++) {
            Brick brick = board->bricks[i][j];

            //se revisa que el bloque exista
            if (brick.state) {
                // defino las coordenadas de los bloques
                float brickx = board->brickoffsetx + board->x + i*BOARD_BRWIDTH;
                float bricky = board->brickoffsety + board->y + j*BOARD_BRHEIGHT;

                // y reviso donde choca la bola
                // http://stackoverflow.com/questions/16198437/minkowski-sum-for-rectangle-intersection-calculation
                // http://gamedev.stackexchange.com/questions/29786/a-simple-2d-rectangle-collision-algorithm-that-also-determines-which-sides-that
                // http://gamedev.stackexchange.com/questions/24078/which-side-was-hit/24091#24091
                float w = 0.5f * (ball->width + BOARD_BRWIDTH);
                float h = 0.5f * (ball->height + BOARD_BRHEIGHT);
                float dx = (ball->x + 0.5f*ball->width) - (brickx + 0.5f*BOARD_BRWIDTH);
                float dy = (ball->y + 0.5f*ball->height) - (bricky + 0.5f*BOARD_BRHEIGHT);

                if (fabs(dx) <= w && fabs(dy) <= h) {
                    // detecta choque
                    board->bricks[i][j].state = false;
                    Mix_PlayChannel( -1, bounce, 0 );
                    scores+=10;


                    float wy = w * dy;
                    float hx = h * dx;

                    if (wy > hx) {
                        if (wy > -hx) {
                            // choque con fondo, se invierte direccion vertical o de y
                            BallBrickResponse(3);
                        } else {
                            // choque por lado izquierdo
                            BallBrickResponse(0);
                        }
                    } else {
                        if (wy > -hx) {
                            // choque por lado derecho
                            BallBrickResponse(2);
                        } else {
                            // choca arriba, se invierte direccion vertical o de y
                            BallBrickResponse(1);
                        }
                    }
                    return;
                }
            }
        }
    }
}

void Game::CheckBrickCollisions2() {// se revisan los choques y se remueven los bloques chocados
    for (int i=0; i<BOARD_WIDTH; i++) {
        for (int j=0; j<BOARD_HEIGHT; j++) {
            Brick brick = board->bricks[i][j];


            if (brick.state) {

                float brickx = board->brickoffsetx + board->x + i*BOARD_BRWIDTH;
                float bricky = board->brickoffsety + board->y + j*BOARD_BRHEIGHT;

                // posicion del centro de la bola
                float ballcenterx = ball->x + 0.5f*ball->width;
                float ballcentery = ball->y + 0.5f*ball->height;

                // centro del bloque
                float brickcenterx = brickx + 0.5f*BOARD_BRWIDTH;
                float brickcentery = bricky + 0.5f*BOARD_BRHEIGHT;

                if (ball->x <= brickx + BOARD_BRWIDTH && ball->x+ball->width >= brickx && ball->y <= bricky + BOARD_BRHEIGHT && ball->y + ball->height >= bricky) {
                    // si hay choque se remueve el bloque
                    board->bricks[i][j].state = false;
                    Mix_PlayChannel( -1, bounce, 0 );
                    scores+=10;



                    // Calculate ysize http://rembound.com/articles/the-breakout-tutorial
                    float ymin = 0;
                    if (bricky > ball->y) {
                        ymin = bricky;
                    } else {
                        ymin = ball->y;
                    }

                    float ymax = 0;
                    if (bricky+BOARD_BRHEIGHT < ball->y+ball->height) {
                        ymax = bricky+BOARD_BRHEIGHT;
                    } else {
                        ymax = ball->y+ball->height;
                    }

                    float ysize = ymax - ymin;

                    // Calculate xsize http://rembound.com/articles/the-breakout-tutorial
                    float xmin = 0;
                    if (brickx > ball->x) {
                        xmin = brickx;
                    } else {
                        xmin = ball->x;
                    }

                    float xmax = 0;
                    if (brickx+BOARD_BRWIDTH < ball->x+ball->width) {
                        xmax = brickx+BOARD_BRWIDTH;
                    } else {
                        xmax = ball->x+ball->width;
                    }

                    float xsize = xmax - xmin;

                    // The origin is at the top-left corner of the screen!
                    // Set collision response
                    if (xsize > ysize) {
                        if (ballcentery > brickcentery) {
                            // Bottom
                            ball->y += ysize + 0.01f; // Move out of collision
                            BallBrickResponse(3);
                        } else {
                            // Top
                            ball->y -= ysize + 0.01f; // Move out of collision
                            BallBrickResponse(1);
                        }
                    } else {
                        if (ballcenterx < brickcenterx) {
                            // Left
                            ball->x -= xsize + 0.01f; // Move out of collision
                            BallBrickResponse(0);
                        } else {
                            // Right
                            ball->x += xsize + 0.01f; // Move out of collision
                            BallBrickResponse(2);
                        }
                    }

                    return;
                }
            }
        }
    }
}
//http://rembound.com/articles/the-breakout-tutorial
void Game::BallBrickResponse(int dirindex) {
    // dirindex 0: Left, 1: Top, 2: Right, 3: Bottom

    // Direction factors
    int mulx = 1;
    int muly = 1;

    if (ball->dirx > 0) {
        // Ball is moving in the positive x direction
        if (ball->diry > 0) {
            // Ball is moving in the positive y direction
            // +1 +1
            if (dirindex == 0 || dirindex == 3) {
                mulx = -1;
            } else {
                muly = -1;
            }
        } else if (ball->diry < 0) {
            // Ball is moving in the negative y direction
            // +1 -1
            if (dirindex == 0 || dirindex == 1) {
                mulx = -1;
            } else {
                muly = -1;
            }
        }
    } else if (ball->dirx < 0) {
        // Ball is moving in the negative x direction
        if (ball->diry > 0) {
            // Ball is moving in the positive y direction
            // -1 +1
            if (dirindex == 2 || dirindex == 3) {
                mulx = -1;
            } else {
                muly = -1;
            }
        } else if (ball->diry < 0) {
            // Ball is moving in the negative y direction
            // -1 -1
            if (dirindex == 1 || dirindex == 2) {
                mulx = -1;
            } else {
                muly = -1;
            }
        }
    }

    // Set the new direction of the ball, by multiplying the old direction
    // with the determined direction factors
    ball->SetDirection(mulx*ball->dirx, muly*ball->diry);
}
//contador de los bloques para saber cuando termina el juego
int Game::GetBrickCount() {
    int brickcount = 0;
    for (int i=0; i<BOARD_WIDTH; i++) {
        for (int j=0; j<BOARD_HEIGHT; j++) {
            Brick brick = board->bricks[i][j];
            if (brick.state) {
                brickcount++;
            }
        }
    }

    return brickcount;
}
//mantengo recreeada la ventana
void Game::Render(float delta, int scores,int lives) {
    SDL_RenderClear(renderer);

    Score->Stadist(scores,lives, press_x);
    //ShowPressX(press_x);
    board->Render(delta);
    paddle->Render(delta);
    ball->Render(delta);


    SDL_RenderPresent(renderer);
}

//void Game::Stadist(){//}int scores,int lives){
  //  Score->Stadist();//scores, lives);
//}