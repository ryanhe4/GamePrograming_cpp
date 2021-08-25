//
// Created by yangheechan on 2021-08-25.
//

#ifndef GAMEPROGRAMING_CPP_GAME_H
#define GAMEPROGRAMING_CPP_GAME_H

#include "SDL2/SDL.h"

class Game {
public:
    Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();
private:
    // helper
    void processInput();
    void updateGame();
    void generateOutput();

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool is_running;
    Uint32 tick_count;
    struct Vector2{
      float x;
      float y;
    };
    Vector2 ball_pos;
    Vector2 paddle_pos;
    int paddle_dir;
};

#endif //GAMEPROGRAMING_CPP_GAME_H
