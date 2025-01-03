#include "buttons.h"

#include "mouse.h"
#include "../utils/gameStatus.h"
#include "../scenes/Menu/menuScene.h"
#include "../scenes/Game/gameScene.h"
#include "../scenes/Game/Enemy/enemy.h"
#include "../scenes/Game/Enemy/waves.h"
#include "../scenes/Game/Friendly/tower.h"
#include "../utils/rectStuff.h"

Button** buttons = NULL;
size_t buttonCount = 0;

void makeButton(Text* text, SDL_Rect rect, SDL_Texture* texture, SDL_Color* highlightColor, char* id, ButtonType type) {
    Button** tmp = realloc(buttons, (buttonCount +1) * sizeof(Button*));
    buttons = tmp;

    Button* temp = malloc(sizeof(Button));
    if (text != NULL) temp->text = text;
    if (highlightColor != NULL) temp->highlightColor = *highlightColor;
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
        }
    }
}

void highlightButtons() {
    for (size_t i = 0; i < buttonCount; i++)
    {
        if (buttons[i]->type == RECTBUTTON) continue;
        if (isMouseOnRect(buttons[i]->rect)) buttons[i]->text->color = buttons[i]->highlightColor;
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
    Button* temp;
    for (size_t i = 0; i < buttonCount; i++)
    {
        if (isMouseOnRect(buttons[i]->rect)) temp = buttons[i];
    }
    
    if (temp->active) {
        if (strcmp(temp->ID, "QuitBtn") == 0) {
            changeScene(NONE);
            gameStatus.running = false;
            freeMenuScene();
        } else if (strcmp(temp->ID, "MenuStartBtn") == 0) {
            freeMenuScene();
            freeButtons();
            initGameScene(renderer, WILLOW);
            changeScene(GAME);
        } else if (strcmp(temp->ID, "nickname") == 0) {
            nicknameValue.color = createColor("DDDDFF", 255);
            memset(nicknameValue.text, 0, 16);
            nickLength = 0;
            temp->active = false;
        } else if (strcmp(temp->ID, "startWave") == 0) {
            startWave();
        } else if (strcmp(temp->ID, "backToMenu") == 0) {
            initMenuScene(renderer);
            changeScene(MENU);
        } else if (strcmp(temp->ID, "waterTower") == 0 || strcmp(temp->ID, "incenseTower") == 0 || strcmp(temp->ID, "crucifixTower") == 0) {
            setTowerUIButtonsState(false);
            createTower(temp->ID);
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
    if (buttonCount == 0) return NULL;
    for (size_t i = 0; i < buttonCount; i++)
    {
        if (strcmp(buttons[i]->ID, buttonId) == 0) return buttons[i];
    }
    return NULL;
}