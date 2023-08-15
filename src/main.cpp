#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

//**************************************************************
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 720;
const string WINDOW_TITLE = "Flappy Bird";

const int FPS = 60;
const int DELAY_TIME = 1000 / FPS;

Uint32 frameStart;
Uint32 frameTime;


SDL_Rect filled_rect;

SDL_Texture* bgImg = NULL;
SDL_Texture* birdImg = NULL;
SDL_Texture* play = NULL;
SDL_Texture* replay = NULL;


void logSDLError(std::ostream& os, 
                 const std::string &msg, bool fatal = false);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();
bool check_collision_two_rects(SDL_Rect rect1,SDL_Rect rect2);
SDL_Texture* loadTexture(SDL_Renderer* renderer, string path);
//**************************************************************



class Bird {
public:
    double x = 200;
    double y = 0;

    double g = 0.5;

    double speed = 0;
    double SPEEDFLY = -10;

    Bird() {
       reset();
    }

    void draw(SDL_Renderer * renderer) {
        SDL_Rect rect = {int(x), int(y), 114, 80};
        SDL_RenderCopy(renderer, birdImg, NULL, &rect);
    }

    SDL_Rect getRect() {
        return {int(x), int(y), 114, 80};
    }
    
    void update() {
        y += speed + 0.5 * g;
        speed += g;
    }

    void jump() {
        speed = SPEEDFLY;
    }

    void reset() {
        x = 200;
        y = 0;

        speed = 0;
    }

    bool drop() {
        return y > SCREEN_HEIGHT - 80;
    }
};

class Column {
public:
    vector<vector<double> > columns;
    int WIDTH = 100;
    int HEIGHT = 500;

    int offWIDTH = 10;
    int offHEIGHT = 50;
    
    int distance = 300;
    double speed = -1.5;
    double a = -0.001;

    int blank = 250;


    Column() {

        reset();
        
    }
    void draw(SDL_Renderer * renderer) {

        for (int i = 0; i < columns.size(); i++) {
            double x = columns[i][0];
            double y = columns[i][1];

            SDL_Rect rect1 = {int(x), int(y) - HEIGHT, WIDTH, HEIGHT};
            SDL_Rect rect2 = {int(x), int(y) + blank, WIDTH, HEIGHT};

            SDL_SetRenderDrawColor(renderer, 3, 252, 157, 255);
            SDL_RenderFillRect(renderer, &rect1);
            SDL_RenderFillRect(renderer, &rect2);
            
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &rect1);
            SDL_RenderDrawRect(renderer, &rect2);

            // vẽ nắp ống

            SDL_Rect rect3 = {int(x) - offWIDTH, int(y) - offHEIGHT, WIDTH + offWIDTH * 2, offHEIGHT};
            SDL_Rect rect4 = {int(x) - offWIDTH, int(y) + blank, WIDTH + offWIDTH * 2, offHEIGHT};

            SDL_SetRenderDrawColor(renderer, 3, 252, 157, 255);
            SDL_RenderFillRect(renderer, &rect3);
            SDL_RenderFillRect(renderer, &rect4);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &rect3);
            SDL_RenderDrawRect(renderer, &rect4);
        }
    }

    void update() {
        for (int i = 0; i < columns.size(); i++) {
            columns[i][0] += speed;

            if (columns[i][0] < -WIDTH) {
                columns.erase(columns.begin() + i);
                make();  
            }

            // if (columns[i][0] <)
        }
        speed += a;
    }

    void make() {
        int x = columns.back()[0] + distance;
        
        int st = 60;
        int en = SCREEN_HEIGHT - blank - 60;

        int y = rand() % (en - st + 1) + st;
        columns.push_back({double(x), double(y)});
    }


    bool checkCollision(Bird * bird) {
        SDL_Rect birdRect = bird->getRect();

        for (int i = 0; i < columns.size(); i++) {
            double x = columns[i][0];
            double y = columns[i][1];

            SDL_Rect rect1 = {int(x), int(y) - HEIGHT, WIDTH, HEIGHT};
            SDL_Rect rect2 = {int(x), int(y) + blank, WIDTH, HEIGHT};

            if (check_collision_two_rects(birdRect, rect1) or check_collision_two_rects(birdRect, rect2)) {
                return true;
            }
        }
        
        return false;
    }
    
    void reset() {
        columns.clear();

        for(int i = 1; i < 5; i++) {
            int x = i * distance + 200;
        
            int st = 120;
            int en = SCREEN_HEIGHT - blank - 120;

            int y = rand() % (en - st + 1) + st;
            columns.push_back({double(x), double(y)});
        }
    }
};

bool check_collision_two_rects(SDL_Rect rect1,SDL_Rect rect2) {
    return rect1.x < rect2.x + rect2.w and
            rect1.x + rect1.w > rect2.x and
            rect1.y < rect2.y + rect2.h and
            rect1.y + rect1.h > rect2.y;
}

enum Scene {
    MENU,
    PLAY,
    REPLAY,
};

Bird bird = Bird();
Column col = Column();
int score = 0;
int scene = MENU;


void refreshScreen(SDL_Window* window, SDL_Renderer* renderer);

int main(int argc, char* argv[]) {
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);

    SDL_Event e;

    bgImg = loadTexture(renderer, "res/img/bg.jpg");
    birdImg = loadTexture(renderer, "res/img/bird.png");
    play = loadTexture(renderer, "res/img/play.png");
    replay = loadTexture(renderer, "res/img/replay.png");
    

    // Xoá toàn bộ màn hình và vẽ
    refreshScreen(window, renderer);

    bool quit = false;

    while (!quit) {
        // Đợi 10 mili giây
        frameStart = SDL_GetTicks();

        while( SDL_PollEvent( &e ) != 0 ) {
            //User requests quit
            if (e.type == SDL_QUIT ) quit = true;
            
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_SPACE) {
                    if (scene == MENU) {
                        scene = PLAY;
                        bird.reset();
                        col.reset();
                    }
                    else if (scene == PLAY) {
                        bird.jump();
                    }
                    else {
                        scene = PLAY;
                        bird.reset();
                        col.reset();
                    }
                }
            }
        }
            
        // Xoá toàn bộ màn hình và vẽ lại
        refreshScreen(window, renderer);

        frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < DELAY_TIME) SDL_Delay(DELAY_TIME - frameTime);
		
    }
    
    quitSDL(window, renderer);
    return 0;
}

// Xoá và vẽ lại toàn bộ màn hình với 1 hình chữ nhật
void refreshScreen(SDL_Window* window, SDL_Renderer* renderer) {
    // Đặt màu vẽ thành xanh lam (blue), xoá màn hình về màu xanh lam.
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);   // blue
    SDL_RenderClear(renderer);

    // Vẽ background
    SDL_RenderCopy(renderer, bgImg, NULL, NULL);

     // Vẽ cột
    col.update();
    col.draw(renderer);

    SDL_Rect rect = {-200, 60, 1350, 619};
    if (scene == MENU) {
        SDL_RenderCopy(renderer, play, NULL, &rect);
    }
    else if (scene == REPLAY) {
        SDL_RenderCopy(renderer, replay, NULL, &rect);
    }
    else {
        // Vẽ flappy Bird
        bird.update();
        bird.draw(renderer);
        
        if (col.checkCollision(&bird) or bird.drop()) {
            scene = REPLAY;
        }
    }

    
   
    SDL_RenderPresent(renderer);
}


//*****************************************************
// Các hàm chung về khởi tạo và huỷ SDL
void logSDLError(std::ostream& os, const std::string &msg, bool fatal) {
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window* &window, SDL_Renderer* &renderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    //window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, 
    //   SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);

    //Khi thông thường chạy với môi trường bình thường ở nhà
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | 
                                              SDL_RENDERER_PRESENTVSYNC);
    //Khi chạy ở máy thực hành WinXP ở trường (máy ảo)
    //renderer = SDL_CreateSoftwareRenderer(SDL_GetWindowSurface(window));
    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer) {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void waitUntilKeyPressed() {
    SDL_Event e;
    while (true) {
        if ( SDL_WaitEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}

SDL_Texture* loadTexture(SDL_Renderer* renderer, string path) {
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL ) {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( newTexture == NULL ) {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}