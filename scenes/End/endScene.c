#include "endScene.h"

#include <SDL2/SDL_ttf.h>

#include "../../utils/text.h"
#include "../../utils/gameStatus.h"
#include "../../utils/rectStuff.h"
#include "../../interactions/buttons.h"

static TTF_Font* font;

Text backToMenu;
SDL_Color backToMenuColor;

Text endMessage;

void initEndScene() {
    font = TTF_OpenFont("../fonts/lazy_dog.ttf", 120);

    if (gameStatus.currentScene == WIN) {
        endMessage.color = createColor("FDD813", 255);
        endMessage.font = font;
        endMessage.rect = createRect(gameStatus.windowSizeX / 2 - 675, gameStatus.windowSizeY / 2 - 250, 1400, 150);
        endMessage.text = "You successfully protected the Truck!";
    } else {
        endMessage.color = createColor("FF0000", 255);
        endMessage.font = font;
        endMessage.rect = createRect(gameStatus.windowSizeX / 2 - 650, gameStatus.windowSizeY / 2 - 250, 1300, 150);
        endMessage.text = "You failed to protect the Truck!";
    }

    backToMenu.text = "Back to Menu";
    backToMenuColor = createColor("999999", 255);
    backToMenu.color = backToMenuColor;
    int w = 350;
    backToMenu.rect = createRect((gameStatus.windowSizeX - w) / 2, 475, w, 100);
    backToMenu.font = font;
    SDL_Color quitHighlightColor = createColor("BBBBBB", 255);
    makeButton(&backToMenu, backToMenu.rect, NULL, &quitHighlightColor, "backToMenu", TEXTBUTTON);
}

void renderEndScene(SDL_Renderer* renderer) {
    renderText(renderer, &endMessage);
    renderText(renderer, &backToMenu);
    backToMenu.color = backToMenuColor;

    highlightButtons();
}