#pragma once
#include "defs.h"

struct Texture {
	Texture(SDL_Texture* tex, int w, int h, int frames);

	SDL_Texture* tex;
	int w;
	int h;
	int frames;
};

class Renderer {
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* target;

public:
	Renderer();
	~Renderer();

	void Blit(double x, double y, int w, int h, double angle, Texture* tex, double scale, int frame, int offset, bool full = false, SDL_RendererFlip flip_flag = SDL_FLIP_NONE);
	void Print(int x, int y, char const* text);
	void DrawLine(int aX, int aY, int bX, int bY);
	void Clear();
	void Present();
	static Texture* GetTexture(texture_e tag);
};
