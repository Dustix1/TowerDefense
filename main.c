// Vložení hlavního hlavičkového souboru SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

#include "utils/rectStuff.h"
#include "utils/gameStatus.h"
#include "utils/text.h"
#include "interactions/buttons.h"
#include "interactions/mouse.h"
#include "scenes/Menu/menuScene.h"
#include "scenes/Game/gameScene.h"
#include "scenes/End/endScene.h"
#include "scenes/Game/Friendly/tower.h"
#include "scenes/Game/Friendly/player.h"

int main()
{
    // Inicializace SDL
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_Rect display;
    SDL_GetDisplayBounds(0, &display);
    gameStatus.windowSizeX = 1920;
    gameStatus.windowSizeY = 1080;
    // Vytvoření okna
    SDL_Window *window = SDL_CreateWindow(
        "Tower Defense", // Titulek okna
        SDL_WINDOWPOS_CENTERED,             // Souřadnice x
        SDL_WINDOWPOS_CENTERED,             // Souřadnice y
        gameStatus.windowSizeX,     // Šířka
        gameStatus.windowSizeY,     // Výška
        SDL_WINDOW_BORDERLESS /*| SDL_WINDOW_FULLSCREEN_DESKTOP*/ // Okno se má po vytvoření rovnou zobrazit
    );

    // scene renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    //SDL_RenderSetLogicalSize(renderer, gameStatus.windowSizeX, gameStatus.windowSizeY);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    SDL_Event event;

    initGame();
    initMenuScene(renderer);

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
                if (isMouseOnButton() && event.button.button == SDL_BUTTON_LEFT) makeButtonsDoSomething(renderer);
                if (event.button.button == SDL_BUTTON_RIGHT) sellTower();
            }
            if (event.type == SDL_MOUSEBUTTONUP) {
                if (draggingTower && event.button.button == SDL_BUTTON_LEFT) stopDragging();
            }

            if (event.type == SDL_KEYDOWN) {
                if (((event.key.keysym.sym >= SDLK_a && event.key.keysym.sym <= SDLK_z) || event.key.keysym.sym == SDLK_SPACE)
                    && !searchForButton("nickname")->active && nickLength < 15) {
                    writeSymbol(event.key.keysym.sym);
                } else if (event.key.keysym.sym == SDLK_RETURN && !searchForButton("nickname")->active && nickLength >= 5) {
                    saveNickname();
                    nicknameValue.color = createColor("DDDDFF", 100);
                    searchForButton("nickname")->active = true;
                } else if (event.key.keysym.sym == SDLK_BACKSPACE && !searchForButton("nickname")->active && nickLength != 0) {
                    delSymbol();
                }
            }
        }


        SDL_RenderClear(renderer);
        if (gameStatus.currentScene == MENU) {
            renderMenu(renderer);
        } else if (gameStatus.currentScene == GAME) {
            renderGame(renderer);
        } else if (gameStatus.currentScene == WIN || gameStatus.currentScene == LOSE){
            renderEndScene(renderer);
        }
        SDL_RenderPresent(renderer);
        
    }

    free(nicknameValue.text);
    nicknameValue.text = NULL;
    free(player.nick);
    player.nick = NULL;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();
    return 0;
}
