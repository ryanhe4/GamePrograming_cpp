//
// Created by yangheechan on 2021-08-25.
//

#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
        :window(nullptr),
         renderer(nullptr),
         is_running(true),
         tick_count(0), paddle_dir(0), paddle_dir2(0)
{
}

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

    paddle_pos.x = 10.0f;
    paddle_pos.y = 768.0f/2.0f;
    paddle_pos2.x = 1024-10.0f-thickness;
    paddle_pos2.y = 768.0f/2.0f;

    Ball ball1 = {{-200.0f, 235.0f},{1024.0f/2.0f, 768.f/2.0f}};
    v_ball.push_back(ball1);
    Ball ball2 = {{200.0f, 235.0f},{1024.0f/2.0f, 768.f/2.0f}};
    v_ball.push_back(ball2);

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

    const auto state = SDL_GetKeyboardState(nullptr);
    if (state[SDL_SCANCODE_ESCAPE]) {
        is_running = false;
    }
    paddle_dir = 0;
    paddle_dir2 = 0;
    if (state[SDL_SCANCODE_W]) {
        paddle_dir -= 1;
    }
    if (state[SDL_SCANCODE_S]) {
        paddle_dir += 1;
    }
    if (state[SDL_SCANCODE_I]) {
        paddle_dir2 -= 1;
    }
    if (state[SDL_SCANCODE_K]) {
        paddle_dir2 += 1;
    }
}
void Game::updateGame()
{
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), tick_count+16));
    float deltaTime = (SDL_GetTicks()-tick_count)/1000.0f;

    if (deltaTime>0.05f) {
        deltaTime = 0.05f;
    }
    // proc paddle
    if (paddle_dir!=0) {
        paddle_pos.y += paddle_dir*300.0f*deltaTime;
        if (paddle_pos.y<(paddleH/2.0f+thickness)) {
            paddle_pos.y = paddleH/2.0f+thickness;
        }
        else if (paddle_pos.y>(768.0f-paddleH/2.0f-thickness)) {
            paddle_pos.y = 768.0f-paddleH/2.0f-thickness;
        }
    }
    if (paddle_dir2!=0) {
        paddle_pos2.y += paddle_dir2*300.0f*deltaTime;
        if (paddle_pos2.y<(paddleH/2.0f+thickness)) {
            paddle_pos2.y = paddleH/2.0f+thickness;
        }
        else if (paddle_pos2.y>(768.0f-paddleH/2.0f-thickness)) {
            paddle_pos2.y = 768.0f-paddleH/2.0f-thickness;
        }
    }

    for(Ball& b:v_ball) {
        Vector2 & ball_pos = b.position;
        Vector2 & ball_vel = b.velocity;

        ball_pos.x += ball_vel.x*deltaTime;
        ball_pos.y += ball_vel.y*deltaTime;

        float diff = paddle_pos.y-ball_pos.y;
        diff = (diff>0.0f) ? diff : -diff;

        if (diff<=paddleH/2.0f and
                ball_pos.x<=25.0f and
                ball_pos.x>=20.0f and
                ball_vel.x<0.0f) {
            ball_vel.x *= -1.0f;
        }
        else if (ball_pos.x<=0.0f ) {
            is_running = false;
        }

        diff = paddle_pos2.y-ball_pos.y;
        diff = (diff>0.0f) ? diff : -diff;

        if (diff<= paddleH/2.0f and
                ball_pos.x <= (1024 - 20.0f) and ball_pos.x>= (1024 - 25.0f) and
                ball_vel.x > 0.0f) {
            ball_vel.x *= -1.0f;
        }
        else if (ball_pos.x >= 1024.0f ) {
            is_running = false;
        }

        // 하단, 상단 처리
        if (ball_pos.y<=thickness and ball_vel.y<0.0f) {
            ball_vel.y *= -1;
        }
        else if (ball_pos.y>=(768.0-thickness) and ball_vel.y>0.0f) {
            ball_vel.y *= -1;
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

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for(auto& ball: v_ball) {
        Vector2& ball_pos = ball.position;

        SDL_Rect ball_rect{
                static_cast<int>(ball_pos.x-thickness/2),
                static_cast<int>(ball_pos.y-thickness/2),
                thickness,
                thickness
        };
        SDL_RenderFillRect(renderer, &ball_rect);
    }

    //draw paddle
    SDL_Rect paddle{
            static_cast<int>(paddle_pos.x),
            static_cast<int>(paddle_pos.y-paddleH/2),
            thickness,
            static_cast<int>(paddleH)
    };
    SDL_RenderFillRect(renderer, &paddle);

    SDL_Rect paddle2{
            static_cast<int>(paddle_pos2.x),
            static_cast<int>(paddle_pos2.y-paddleH/2),
            thickness,
            static_cast<int>(paddleH)
    };
    SDL_RenderFillRect(renderer, &paddle2);

    SDL_RenderPresent(renderer);
}
