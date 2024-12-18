#include "buttons.h"

#include "mouse.h"
#include "../utils/gameStatus.h"
#include "../scenes/Menu/menuScene.h"
#include "../scenes/Game/gameScene.h"
#include "../scenes/Game/Enemy/enemy.h"
#include "../scenes/Game/Enemy/waves.h"
#include "../utils/rectStuff.h"

Button** buttons = NULL;
size_t buttonCount = 0;

void makeButton(Text* text, SDL_Rect rect, SDL_Texture* texture, SDL_Color* hilightColor, char* id, ButtonType type) {
    Button** tmp = realloc(buttons, (buttonCount +1) * sizeof(Text*));
    buttons = tmp;

    Button* temp = malloc(sizeof(Button));
    if (text != NULL) temp->text = text;
    if (hilightColor != NULL) temp->hilightColor = *hilightColor;
    if (texture != NULL) temp->texture = texture;
    temp->rect = rect;
    temp->ID = id;
    temp->type = type;
    temp->active = true;

    buttons[buttonCount] = temp;
    buttonCount++;
}

void makeButtonsLookInActive() {
    for (size_t i = 0; i < buttonCount; i++)
    {
        if (buttons[i]->type == RECTBUTTON) {
            if (!buttons[i]->active) SDL_SetTextureColorMod(buttons[i]->texture, 100, 100, 100);
            else SDL_SetTextureColorMod(buttons[i]->texture, 255, 255, 255);
        } else {
            if (!buttons[i]->active) {
                SDL_Color inactiveColor;
                SDL_Color buttonColor = buttons[i]->text->color;
                if (buttonColor.r >= 100) inactiveColor.r = buttonColor.r - 100;
                else inactiveColor.r = 0;
                if (buttonColor.g >= 100) inactiveColor.g = buttonColor.g - 100;
                else inactiveColor.g = 0;
                if (buttonColor.b >= 100) inactiveColor.b = buttonColor.b - 100;
                else inactiveColor.b = 0;
                inactiveColor.a = buttonColor.a;
            }
        }
    }
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
    
    if (temp->active) {
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
}

void freeButtons() {
    for (size_t i = 0; i < buttonCount; i++) {
        free(buttons[i]);
    }
    buttonCount = 0;
    free(buttons);
    buttons = NULL;
}

Button* searchForButton(char* buttonId) {
    for (size_t i = 0; i < buttonCount; i++)
    {
        if (strcmp(buttons[i]->ID, buttonId) == 0) return buttons[i];
    }
    return NULL;
}