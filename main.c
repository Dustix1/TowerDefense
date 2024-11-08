// Vložení hlavního hlavičkového souboru SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "utils/rectStuff.h"
#include "utils/gameStatus.h"
#include "utils/text.h"
#include "interactions/mouse.h"
#include "scenes/Menu/menuScene.h"

int main(int argc, char *argv[])
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

    // Menu scene renderer
    SDL_Renderer *menuRenderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(menuRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    // Game Scene renderer
    SDL_Renderer *gameRenderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(gameRenderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    SDL_Event event;
    int running = 1;

    initGame();
    initMenuScene();

    while (running == 1)
    {
        // Dokud jsou k dispozici nějaké události, ukládej je do proměnné `event`
        while (SDL_PollEvent(&event))
        {
            // Pokud došlo k uzavření okna, nastav proměnnou `running` na `0`
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                
            }
        }

        if (gameStatus.currentScene == MENU)
        {
            renderMenu(menuRenderer, windowSizeX, windowSizeY);
            SDL_RenderPresent(menuRenderer);
        }
        else
        {
            SDL_RenderPresent(gameRenderer);
        }
    }

    // Uvolnění prostředků
    SDL_DestroyRenderer(menuRenderer);
    SDL_DestroyRenderer(gameRenderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();

    return 0;
}
