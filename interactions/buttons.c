#include "buttons.h"

#include "mouse.h"
#include "../utils/gameStatus.h"
#include "../scenes/Menu/menuScene.h"
#include "../scenes/Game/gameScene.h"
#include "../scenes/Game/Enemy/enemy.h"
#include "../scenes/Game/Enemy/waves.h"

Button** buttons = NULL;
size_t buttonCount = 0;

void makeButton(Text* text, SDL_Rect rect, SDL_Color* hilightColor, char* id, ButtonType type) {
    Button** tmp = realloc(buttons, (buttonCount +1) * sizeof(Text*));
    buttons = tmp;

    Button* temp = malloc(sizeof(Button));
    if (text != NULL) temp->text = text;
    if (hilightColor != NULL) temp->hilightColor = *hilightColor;
    temp->rect = rect;
    temp->ID = id;
    temp->type = type;

    buttons[buttonCount] = temp;
    buttonCount++;
}

void highlightButtons() {
    for (size_t i = 0; i < buttonCount; i++)
    {
        if (buttons[i]->type == RECTBUTTON) continue;
        if (isMouseOnRect(buttons[i]->rect)) buttons[i]->text->color = buttons[i]->hilightColor;
    }
}

bool isMouseOnButton() {
    for (size_t i = 0; i < buttonCount; i++)
    {
        if (isMouseOnRect(buttons[i]->rect)) return true;
    }
    return false;
}

void makeButtonsDoSomething(SDL_Renderer* renderer) {
    Button* temp = malloc(sizeof(Button));
    for (size_t i = 0; i < buttonCount; i++)
    {
        if (isMouseOnRect(buttons[i]->rect)) temp = buttons[i];
    }
    
    if (strcmp(temp->ID, "QuitBtn") == 0) {
        gameStatus.running = false;
    } else if (strcmp(temp->ID, "MenuStartBtn") == 0) {
        freeMenuScene();
        freeButtons();
        initGameScene(renderer, WILLOW); // MAKE SELECTION POSSIBLE LATER
        changeScene(GAME);
    } else if (strcmp(temp->ID, "spwnRndEne") == 0) {
        spawnNewEnemy(rand() % 5);
    } else if (strcmp(temp->ID, "startWave") == 0) {
        startWave();
    }
}

void freeButtons() {
    for (size_t i = 0; i < buttonCount; i++) {
        free(buttons[i]);
    }
    buttonCount = 0;
    free(buttons);
    buttons = NULL;
}