// Vložení hlavního hlavičkového souboru SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "utils/rectStuff.h"
#include "utils/gameStatus.h"
#include "utils/text.h"
#include "interactions/mouse.h"

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

    TTF_Font *font = TTF_OpenFont("../fonts/Arial.ttf", 20);

    SDL_Event event;
    int running = 1;

    initGame();

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
        }

        if (gameStatus.currentScene == MENU)
        {
            SDL_Rect startBtnRect = createRect(windowSizeX / 2 - 125, 75, 300, 100);
            char startBtnColor[7] = {0};
            if (!isMouseOnRect(startBtnRect)) {
                strcpy(startBtnColor, "0000FF");
            }
            else {
                strcpy(startBtnColor, "00FF00");
            }
            sdl_draw_text(menuRenderer, font, createColor(startBtnColor, 255), startBtnRect, "START");
            SDL_RenderPresent(menuRenderer);
        }
        else
        {

            SDL_RenderPresent(gameRenderer);
        }
    }

    // Uvolnění prostředků
    SDL_DestroyRenderer(menuRenderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_CloseFont(font);
    TTF_Quit();

    return 0;
}
