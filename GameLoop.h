#pragma once
#include <SDL.h>
#include <iostream>

using namespace std;

class GameLoop
{
private:
    const int HEIGHT = 640;
    const int WIDTH = 800;
    bool GameState;
    bool birdJumping;
    SDL_Window* window;
    SDL_Event event1;
    SDL_Renderer* renderer;
    SDL_Rect birdRect; // Toàn bộ hình chữ nhật con chim
    SDL_Rect upperPipeRect; // Hình chữ nhật ống trên
    SDL_Rect lowerPipeRect; // Hình chữ nhật ống dưới
public:
    GameLoop();
    ~GameLoop();
    bool getGameState();
    void Initialize();
    void Event();
    void Render();
    void Update();
    void Clear();
};