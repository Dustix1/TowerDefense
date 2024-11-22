#include "textButtons.h"

#include "mouse.h"
#include "../utils/gameStatus.h"
#include "../scenes/Menu/menuScene.h"
#include "../scenes/Game/gameScene.h"

Button** textButtons = NULL;
size_t buttonCount = 0;

void makeButton(Text* text, SDL_Color hilightColor, char* id) {
    Button** tmp = realloc(textButtons, (buttonCount +1) * sizeof(Text*));
    textButtons = tmp;

    Button* temp = malloc(sizeof(Button));
    temp->text = text;
    temp->hilightColor = hilightColor;
    temp->ID = id;

    textButtons[buttonCount] = temp;
    buttonCount++;
}

void highlightButtons() {
    for (size_t i = 0; i < buttonCount; i++)
    {
        if (isMouseOnRect(textButtons[i]->text->rect)) textButtons[i]->text->color = textButtons[i]->hilightColor;
    }
}

bool isMouseOnButton() {
    for (size_t i = 0; i < buttonCount; i++)
    {
        if (isMouseOnRect(textButtons[i]->text->rect)) return true;
    }
    return false;
}

void makeButtonsDoSomething(SDL_Renderer* renderer) {
    Button* temp = malloc(sizeof(Button));
    for (size_t i = 0; i < buttonCount; i++)
    {
        if (isMouseOnRect(textButtons[i]->text->rect)) temp = textButtons[i];
    }
    
    if (strcmp(temp->ID, "QuitBtn") == 0) {
        gameStatus.running = false;
    } else if (strcmp(temp->ID, "MenuStartBtn") == 0) {
        freeMenuScene();
        freeTextButtons();
        initGameScene(renderer, WILLOW); // MAKE SELECTION POSSIBLE LATER
        changeScene(GAME);
    }
}

void freeTextButtons() {
    for (size_t i = 0; i < buttonCount; i++) {
        free(textButtons[i]);
    }
    buttonCount = 0;
    free(textButtons);
    textButtons = NULL;
}