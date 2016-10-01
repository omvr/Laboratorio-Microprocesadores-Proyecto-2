#include "main.h"



int main(int argc, char* argv[]) {


    // Create the game object
    Game* game = new Game();

    // Initialize and run the game
    if (game->Init()) {
        game->Bienvenida();
        game->Run();
        game->Salida();
    }
    // Clean up

    delete game;
    //getchar();
    return 0;

}