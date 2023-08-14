#include "GameLoop.h"

int main(int argc, char** argv)
{
    GameLoop* g = new GameLoop();
    g->Initialize();
    while (g->getGameState())
    {
        g->Update();
        g->Render();
        g->Event();
    }
    g->Clear();
    delete g;
    return 0;
}