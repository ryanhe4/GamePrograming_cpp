//
// Created by yangheechan on 2021-08-25.
//

#ifndef GAMEPROGRAMING_CPP_GAME_H
#define GAMEPROGRAMING_CPP_GAME_H

#include <vector>
#include "SDL2/SDL.h"

struct Vector2{
  float x;
  float y;
};

struct Ball {
  Vector2 velocity;
  Vector2 position;
};

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
    Uint32 tick_count;
    bool is_running;

//    Vector2 ball_pos;
    Vector2 paddle_pos;
    Vector2 paddle_pos2;
//    Vector2 ball_vel;

    std::vector<Ball> v_ball;

    int paddle_dir;
    int paddle_dir2;
};

#endif //GAMEPROGRAMING_CPP_GAME_H
