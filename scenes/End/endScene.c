#include "endScene.h"

#include <SDL2/SDL_ttf.h>

#include "../../utils/text.h"
#include "../../utils/gameStatus.h"
#include "../../utils/rectStuff.h"
#include "../../interactions/buttons.h"
#include "../Game/Friendly/player.h"
#include "../../save/save.h"

static TTF_Font* font;

Text backToMenu;
SDL_Color backToMenuColor;

Text endMessage;
Text scoreText;
Text playerNick;

void initEndScene() {
    font = TTF_OpenFont("../fonts/lazy_dog.ttf", 120);

    saveData();

    if (gameStatus.currentScene == WIN) {
        endMessage.color = createColor("FDD813", 255);
        endMessage.font = font;
        endMessage.rect = createRect(gameStatus.windowSizeX / 2 - 675, gameStatus.windowSizeY / 2 - 215, 1400, 150);
        endMessage.text = "You successfully protected the Truck!";
    } else {
        endMessage.color = createColor("FF0000", 255);
        endMessage.font = font;
        endMessage.rect = createRect(gameStatus.windowSizeX / 2 - 675, gameStatus.windowSizeY / 2 - 215, 1300, 150);
        endMessage.text = "You failed to protect the Truck!";
    }

    playerNick.color = createColor("FFFFFF", 255);
    playerNick.font = font;
    playerNick.rect = createRect(gameStatus.windowSizeX /2 - 350, gameStatus.windowSizeY / 2 - 50, 300, 100);
    playerNick.text = malloc(16 * sizeof(char));
    memcpy(playerNick.text, player.nick, strlen(player.nick) + 1);

    scoreText.color = createColor("FFFFFF", 255);
    scoreText.font = font;
    scoreText.rect = createRect(playerNick.rect.x + playerNick.rect.w + 100, playerNick.rect.y, 500, 100);
    scoreText.text = malloc(50 * sizeof(char));
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
    renderText(renderer, &playerNick);
    backToMenu.color = backToMenuColor;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
    SDL_RenderFillRect(renderer, &(SDL_Rect){gameStatus.windowSizeX / 2 - 5, gameStatus.windowSizeY / 2 - 70, 10, 125});
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    highlightButtons();
}

void freeEndScene() {
    free(scoreText.text);
    free(playerNick.text);
    playerNick.text = NULL;
    TTF_CloseFont(font);
}