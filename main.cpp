#include <iostream>
#include "Game.h"

int main(int argv, char** args)
{
    Game game;
    bool success = game.Initialize();
    if(success) {
        game.RunLoop();
    }
    game.Shutdown();
    return 0;
}
