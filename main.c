// Vložení hlavního hlavičkového souboru SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "utils/rectStuff.h"
#include "utils/gameStatus.h"
#include "utils/text.h"
#include "interactions/textButtons.h"
#include "interactions/mouse.h"
#include "scenes/Menu/menuScene.h"
#include "scenes/Game/gameScene.h"

int main()
{
    // Inicializace SDL
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    int windowSizeX = 1500;
    int windowSizeY = 1000;
    // Vytvoření okna
    SDL_Window *window = SDL_CreateWindow(
        "Tower Defense", // Titulek okna
        500,             // Souřadnice x
        500,             // Souřadnice y
        windowSizeX,     // Šířka
        windowSizeY,     // Výška
        SDL_WINDOW_SHOWN // Okno se má po vytvoření rovnou zobrazit
    );

    // scene renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    SDL_Event event;

    initGame();
    initMenuScene();

    while (gameStatus.running)
    {
        // Dokud jsou k dispozici nějaké události, ukládej je do proměnné `event`
        while (SDL_PollEvent(&event))
        {
            // Pokud došlo k uzavření okna, nastav proměnnou `running` na `0`
            if (event.type == SDL_QUIT)
            {
                gameStatus.running = false;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (isMouseOnButton()) makeButtonsDoSomething(renderer);
            }
        }

        SDL_GetWindowSize(window, &windowSizeX, &windowSizeY);

        SDL_RenderClear(renderer);
        if (gameStatus.currentScene == MENU) {
            renderMenu(renderer, windowSizeX, windowSizeY);
        } else {
            renderGame(renderer, windowSizeX, windowSizeY);
        }
        SDL_RenderPresent(renderer);
        
    }

    // Uvolnění prostředků
    if (gameStatus.currentScene == MENU) freeMenuScene();
    freeTextButtons();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();

    return 0;
}
