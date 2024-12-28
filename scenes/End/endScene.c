#include "endScene.h"

#include <SDL2/SDL_ttf.h>

#include "../../utils/text.h"
#include "../../utils/gameStatus.h"
#include "../../utils/rectStuff.h"
#include "../../interactions/buttons.h"
#include "../Game/Friendly/player.h"

static TTF_Font* font;

Text backToMenu;
SDL_Color backToMenuColor;

Text endMessage;
Text scoreText;

void initEndScene() {
    font = TTF_OpenFont("../fonts/lazy_dog.ttf", 120);

    if (gameStatus.currentScene == WIN) {
        endMessage.color = createColor("FDD813", 255);
        endMessage.font = font;
        endMessage.rect = createRect(gameStatus.windowSizeX / 2 - 675, gameStatus.windowSizeY / 2 - 115, 1400, 150);
        endMessage.text = "You successfully protected the Truck!";
    } else {
        endMessage.color = createColor("FF0000", 255);
        endMessage.font = font;
        endMessage.rect = createRect(gameStatus.windowSizeX / 2 - 650, gameStatus.windowSizeY / 2 - 115, 1300, 150);
        endMessage.text = "You failed to protect the Truck!";
    }

    scoreText.color = createColor("FFFFFF", 255);
    scoreText.font = font;
    scoreText.rect = createRect(gameStatus.windowSizeX /2 - 250, gameStatus.windowSizeY / 2 - 215, 500, 100);
    scoreText.text = malloc(50 * sizeof(char));
    if (player.money != 0) player.score *= player.money / 4;
    sprintf(scoreText.text, "SCORE: %d", player.score);

    backToMenu.text = "Back to Menu";
    backToMenuColor = createColor("999999", 255);
    backToMenu.color = backToMenuColor;
    int w = 350;
    backToMenu.rect = createRect((gameStatus.windowSizeX - w) / 2, gameStatus.windowSizeY / 2 + 50, w, 100);
    backToMenu.font = font;
    SDL_Color quitHighlightColor = createColor("BBBBBB", 255);
    makeButton(&backToMenu, backToMenu.rect, NULL, &quitHighlightColor, "backToMenu", TEXTBUTTON);
}

void renderEndScene(SDL_Renderer* renderer) {
    renderText(renderer, &endMessage);
    renderText(renderer, &backToMenu);
    renderText(renderer, &scoreText);
    backToMenu.color = backToMenuColor;

    highlightButtons();
}

void freeEndScene() {
    free(scoreText.text);
    TTF_CloseFont(font);
}