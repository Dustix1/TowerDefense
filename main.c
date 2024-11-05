// Vložení hlavního hlavičkového souboru SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "utils/rectStuff.h"

void sdl_draw_text(SDL_Renderer *renderer, TTF_Font *font, SDL_Color color, SDL_Rect location, const char *text)
{
    // Vykreslení textu se zadaným fontem a barvou do obrázku (surface)
    SDL_Surface *surface = TTF_RenderText_Blended(font, text, color);
    // Převod surface na hardwarovou texturu
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Vykreslení obrázku
    SDL_RenderCopy(renderer, texture, NULL, &location);

    // Uvolnění textury a surface
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

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
        500,               // Souřadnice x
        500,               // Souřadnice y
        windowSizeX,       // Šířka
        windowSizeY,       // Výška
        SDL_WINDOW_SHOWN   // Okno se má po vytvoření rovnou zobrazit
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

    SDL_Texture *image = IMG_LoadTexture(menuRenderer, "../images/gru.png");



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


        int x, y;
        SDL_GetMouseState(&x, &y);

        SDL_Rect bg = createRect(0, 0, x, y);
        SDL_RenderCopy(menuRenderer, image, NULL, &bg);

        SDL_Color startColor = createColor(x, y, x, 255);

        SDL_Rect rect = createRect(windowSizeX / 2 - 75, 175, 300, 200);

        const char *kokot = "Test haha";
        sdl_draw_text(menuRenderer, font, startColor, rect, kokot);



        // Zobraz vykreslené prvky na obrazovku
        SDL_RenderPresent(menuRenderer);
    }



    // Uvolnění prostředků
    SDL_DestroyTexture(image);
    SDL_DestroyRenderer(menuRenderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_CloseFont(font);
    TTF_Quit();

    return 0;
}
