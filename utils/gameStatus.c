#include "gameStatus.h"

#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL.h>

struct GameStatusStruct gameStatus;

static Uint64 last;
static Uint64 now;
double deltaTime;

void changeScene(currentSceneEnum scene) {
    gameStatus.currentScene = scene;
}

void initGame() {
    changeScene(MENU);
    gameStatus.running = true;
    last = SDL_GetPerformanceCounter();
}

void updateDeltaTime() {
    now = SDL_GetPerformanceCounter();
    
    // Výpočet delty, času od posledního provedení tohoto řádku (tj. délky iterace herní smyčky)
    deltaTime = (double)((now - last) / (double)SDL_GetPerformanceFrequency());

    // Uložení poslední hodnoty čítače
    last = now;
}