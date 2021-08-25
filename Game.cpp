//
// Created by yangheechan on 2021-08-25.
//

#include "Game.h"
Game::Game()
        :window(nullptr),
         is_running(true),
         ball_pos{1024/2, 768/2},
         paddle_pos{25, 768/2-50}
{

}

const int thickness = 15;
const int paddleH = thickness * 7;
bool Game::Initialize()
{
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult!=0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }
    window = SDL_CreateWindow(
            "Game Programming in C++ (Chapter 1)",
            100,
            100,
            1024,
            768,
            0
    );
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }
    renderer = SDL_CreateRenderer(
            window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }
    return true;
}
void Game::Shutdown()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
void Game::RunLoop()
{
    while (is_running) {
        processInput();
        updateGame();
        generateOutput();
    }
}
void Game::processInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:is_running = false;
            break;
        }
    }

    const auto state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE]) {
        is_running = false;
    }
    paddle_dir = 0;
    if (state[SDL_SCANCODE_W]) {
        paddle_dir -= 1;
    }
    if (state[SDL_SCANCODE_S]){
        paddle_dir += 1;
    }
}
void Game::updateGame()
{
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), tick_count+16));
    float deltaTime = (SDL_GetTicks()-tick_count)/1000.0f;

    if (deltaTime>0.05f) {
        deltaTime = 0.05f;
    }
    if(paddle_dir != 0){
        paddle_pos.y += paddle_dir* 300.0f * deltaTime;
        if(paddle_pos.y < (paddleH/2.0f + thickness)) {
            paddle_pos.y = paddleH/2.0f + thickness;
        } else if(paddle_pos.y > (768.0f - paddleH/2.0f - thickness)){
            paddle_pos.y = 768.0f-paddleH/2.0f-thickness;
        }

    }

    tick_count = SDL_GetTicks();
}
void Game::generateOutput()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
    SDL_Rect wall{0, 0, 1024, thickness};
    SDL_RenderFillRect(renderer, &wall);
    wall.x = 0, wall.y = 768-thickness;
    SDL_RenderFillRect(renderer, &wall);
    wall.x = 1024-thickness, wall.y = 0, wall.h = 768, wall.w = thickness;
    SDL_RenderFillRect(renderer, &wall);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect ball{
            static_cast<int>(ball_pos.x-thickness/2),
            static_cast<int>(ball_pos.y-thickness/2),
            thickness,
            thickness
    };
    SDL_RenderFillRect(renderer, &ball);

    SDL_Rect paddle{
            static_cast<int>(paddle_pos.x-thickness/2),
            static_cast<int>(paddle_pos.y-thickness/2),
            thickness,
            paddleH
    };
    SDL_RenderFillRect(renderer, &paddle);

    SDL_RenderPresent(renderer);
}
