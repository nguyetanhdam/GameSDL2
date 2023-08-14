#include "GameLoop.h"
#include <cstdlib>

GameLoop::GameLoop()
{
    window = NULL;
    renderer = NULL;
    GameState = false;
    birdJumping = false;

    // size & position of bird
    birdRect.x = WIDTH / 4;
    birdRect.y = HEIGHT / 2;
    birdRect.w = 30;
    birdRect.h = 30;

    // size & position of pipe
    upperPipeRect.x = WIDTH;
    upperPipeRect.y = 0;
    upperPipeRect.w = 50;
    upperPipeRect.h = (8 * HEIGHT / 10) / 2; // Chiều cao của ống trên

    lowerPipeRect.x = WIDTH;
    lowerPipeRect.y = upperPipeRect.h + 2 * birdRect.h; // khoảng cách giữa ống trên và ống dưới
    lowerPipeRect.w = 50;
    lowerPipeRect.h = (8 * HEIGHT / 10) / 2 - 2 * HEIGHT / 10; // Chiều cao của ống dưới
}

GameLoop::~GameLoop()
{
}

bool GameLoop::getGameState()
{
    return GameState;
}

void GameLoop::Initialize()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
    if (window)
    {
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer)
        {
            cout << "Succeeded!" << endl;
            GameState = true;
        }
        else
        {
            cout << "Not created!" << endl;
        }
    }
    else
    {
        cout << "Window not created!" << endl;
    }
}

void GameLoop::Event()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            GameState = false;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.sym == SDLK_SPACE)
            {
                birdJumping = true;
            }
        }
        else if (e.type == SDL_KEYUP)
        {
            if (e.key.keysym.sym == SDLK_SPACE)
            {
                birdJumping = false;
            }
        }
    }
}

void GameLoop::Render()
{
    SDL_RenderClear(renderer);

    // Vẽ bầu trời
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // Màu xanh dương nhạt
    SDL_RenderFillRect(renderer, NULL);

    // Vẽ thảm cỏ
    SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255); // Màu xanh lá cây
    SDL_Rect grassRect = { 0, 8 * HEIGHT / 10, WIDTH, 2 * HEIGHT / 10 };
    SDL_RenderFillRect(renderer, &grassRect);

    // Vẽ con chim
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Màu đỏ
    SDL_RenderFillRect(renderer, &birdRect);

    // Vẽ ống trên và ống dưới
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Màu đen
    SDL_RenderFillRect(renderer, &upperPipeRect);
    SDL_RenderFillRect(renderer, &lowerPipeRect);

    SDL_RenderPresent(renderer);
}

void GameLoop::Update()
{
    if (birdJumping)
    {
        birdRect.y -= 5; // Di chuyển lên
    }
    else
    {
        birdRect.y += 2; // Di chuyển xuống
    }

    // Di chuyển ống sang trái
    upperPipeRect.x -= 5;
    lowerPipeRect.x -= 5;

    // nếu ống đi qua khỏi màn hình, ta sẽ tạo lại ống mới
    if (upperPipeRect.x < -50)
    {
        upperPipeRect.x = WIDTH;
        upperPipeRect.h = HEIGHT / 3 + rand() % 150;
        lowerPipeRect.x = WIDTH;
        lowerPipeRect.y = upperPipeRect.h + 200;
        lowerPipeRect.h = HEIGHT - lowerPipeRect.y;
    }
}

void GameLoop::Clear()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}