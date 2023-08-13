#pragma once
#include <SDL.h>
#include <iostream>

using namespace std;

class GameLoop
{
private:
	const int HEIGHT = 640;
	const int WIDTH = 800;
	SDL_Window* window;
	SDL_Renderer* renderer;
public:
	GameLoop();
	void Intialize();
	void Event();
	void Render();
	void Clear();
};