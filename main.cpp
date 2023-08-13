#include <SDL.h>

/*
void DrawRectangle(SDL_Renderer* renderer, int x, int y, int width, int height, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = { x, y, width, height };
    SDL_RenderFillRect(renderer, &rect);
}

void DrawTriangle(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int x3, int y3, SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Point points[4] = { { x1, y1 }, { x2, y2 }, { x3, y3 }, { x1, y1 } };
    SDL_RenderDrawLines(renderer, points, 4);
}
*/

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Flappy Bird", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);

    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_RenderClear(renderer);

        // Vẽ bầu trời
        SDL_Rect skyRect = { 0, 0, 800, 500 };
        SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255); // Màu xanh dương nhạt
        SDL_RenderFillRect(renderer, &skyRect);

        /*
        // Vẽ các tòa nhà
        for (int i = 0; i < 5; ++i) {
            int houseX = 70 + i * 140; // Khoảng cách giữa các ngôi nhà
            SDL_Color houseColor = { 255, 160, 160, 255 };
            DrawRectangle(renderer, houseX, 150, 50, 30, houseColor);

            // Màu mái nhà khác nhau cho từng ngôi nhà
            SDL_Color roofColor;
            if (i % 2 == 0) {
                roofColor = { 100, 100, 100, 255 }; // Màu xám
            }
            else {
                roofColor = { 255, 0, 0, 255 }; // Màu đỏ
            }

            DrawTriangle(renderer, houseX, 300, houseX + 50, 200, houseX + 100, 300, roofColor);
        }
        */
        // Thảm cỏ xanh lá cây
        SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255); // Màu xanh lá cây

        // Hiển thị lên cửa sổ
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
