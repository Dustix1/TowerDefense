// Vložení hlavního hlavičkového souboru SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

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
        100,               // Souřadnice x
        100,               // Souřadnice y
        windowSizeX,       // Šířka
        windowSizeY,       // Výška
        SDL_WINDOW_SHOWN   // Okno se má po vytvoření rovnou zobrazit
    );

    // Vytvoření kreslítka
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    TTF_Font *font = TTF_OpenFont("../fonts/Arial.ttf", 20);

    SDL_Event event;
    int running = 1;

    SDL_Texture *image = IMG_LoadTexture(renderer, "../images/gru.png");

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

        SDL_Rect bg = {
            .x = 0,
            .y = 0,
            .w = windowSizeX,
            .h = windowSizeY};
        SDL_RenderCopy(renderer, image, NULL, &bg);

        int x, y;
        SDL_GetMouseState(&x, &y);
        SDL_Color startColor = {
            .b = 0,
            .g = 0,
            .r = 255,
            .a = 255};
        SDL_Rect rect = {
            .x = windowSizeX / 2 - 75,
            .y = 175,
            .w = 300,
            .h = 200};
        const char *kokot = "Test haha";
        sdl_draw_text(renderer, font, startColor, rect, kokot);

        // Zobraz vykreslené prvky na obrazovku
        SDL_RenderPresent(renderer);
    }

    // Uvolnění prostředků
    SDL_DestroyTexture(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_CloseFont(font);
    TTF_Quit();

    return 0;
}
