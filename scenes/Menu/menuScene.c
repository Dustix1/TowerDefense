#include "menuScene.h"

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "../../utils/rectStuff.h"
#include "../../interactions/buttons.h"
#include "../../interactions/mouse.h"
#include "../../utils/gameStatus.h"
#include "../Game/Friendly/player.h"
#include "../../save/save.h"

static TTF_Font *font;
Text title;
Text startBtnText;
Text quitBtnText;
SDL_Color quitColor;
SDL_Color startColor;
SDL_Color titleColor;

Text nicknameLabel;
Text nicknameValue;
SDL_Rect nicknameBgButton;

Text pressEnterText;
Text minCharsText;

Text leaderboardLabel;
Text leaderboardFirst;
Text leaderboardSecond;
Text leaderboardThird;
Text leaderboardFourth;
Text leaderboardFifth;

Text controlsText;
Text waterTowerText;
Text crucifixTowerText;
Text IncenseTowerText;

SDL_Texture* holyWaterCannonTexture;
SDL_Texture* crucifixTexture;
SDL_Texture* incenseTexture;
SDL_Texture* background;

SDL_Rect backgroundRect;
SDL_Rect waterCannonImage;
SDL_Rect crucifixImage;
SDL_Rect incenseImage;

int nickLength = 0;
static bool alreadyGotName = false;

static int saveCount = 0;
char** leaderboard = NULL;

void initMenuScene(SDL_Renderer* renderer)
{
    font = TTF_OpenFont("../fonts/lazy_dog.ttf", 120);

    background = IMG_LoadTexture(renderer, "../scenes/Menu/TarotBackground.jpg");
    backgroundRect = createRect(-200, 0, gameStatus.windowSizeX + 400, gameStatus.windowSizeY + 200);

    if (!alreadyGotName) player.nick = malloc(16 * sizeof(char));
    if (!alreadyGotName) nicknameValue.text = malloc(16 * sizeof(char));
    if (!alreadyGotName) nicknameValue.text[15] = '\0';

    int w;
    title.text = "Phasmophobia Tower Defense";
    titleColor = createColor("FFFFFF", 255);
    title.color = titleColor;
    title.rect = createRect(125, 50, gameStatus.windowSizeX - 275, 125);
    title.font = font;

    startBtnText.text = "Start";
    startColor = createColor("00CC00", 255);
    startBtnText.color = startColor;
    w = 300;
    startBtnText.rect = createRect((gameStatus.windowSizeX - w) / 2, 225, w, 125);
    startBtnText.font = font;
    SDL_Color startHighlightColor = createColor("00FF00", 255);
    makeButton(&startBtnText, startBtnText.rect, NULL, &startHighlightColor, "MenuStartBtn", TEXTBUTTON);

    quitBtnText.text = "Quit";
    quitColor = createColor("BB0000", 255);
    quitBtnText.color = quitColor;
    w = 300;
    quitBtnText.rect = createRect((gameStatus.windowSizeX - w) / 2, startBtnText.rect.y + startBtnText.rect.h + 25, w, 125);
    quitBtnText.font = font;
    SDL_Color quitHighlightColor = createColor("FF0000", 255);
    makeButton(&quitBtnText, quitBtnText.rect, NULL, &quitHighlightColor, "QuitBtn", TEXTBUTTON);

    nicknameLabel.text = "Nickname:";
    nicknameLabel.color = createColor("FFFFFF", 255);
    nicknameLabel.font = font;
    nicknameLabel.rect = createRect(startBtnText.rect.x + startBtnText.rect.w + 300, startBtnText.rect.y, 300, 100);

    nicknameBgButton = createRect(nicknameLabel.rect.x - 50, nicknameLabel.rect.y + nicknameLabel.rect.h + 25,
                        nicknameLabel.rect.w + 100, 150);
    makeButton(NULL, nicknameBgButton, NULL, NULL, "nickname", RECTBUTTON);

    nicknameValue.font = font;
    nicknameValue.color = createColor("DDDDFF", 100);
    nicknameValue.rect = createRect(nicknameBgButton.x + 5, nicknameBgButton.y + 15, nicknameBgButton.w - 10, nicknameBgButton.h - 30);
    if (!alreadyGotName) sprintf(nicknameValue.text, "Enter Nickname");
    else memcpy(nicknameValue.text, player.nick, strlen(player.nick) + 1);

    minCharsText.color = createColor("FFFF00", 125);
    minCharsText.font = font;
    minCharsText.rect = createRect(nicknameBgButton.x, nicknameBgButton.y + nicknameBgButton.h + 25, nicknameBgButton.w - 50, 50);
    minCharsText.text = "Min 5 characters.";

    pressEnterText.color = createColor("FFFFFF", 125);
    pressEnterText.font = font;
    pressEnterText.rect = createRect(minCharsText.rect.x, minCharsText.rect.y + minCharsText.rect.h + 15, nicknameBgButton.w, 50);
    pressEnterText.text = "Press ENTER to save";

    leaderboardLabel.font = font;
    leaderboardLabel.color = createColor("FFFFFF", 255);
    leaderboardLabel.rect = createRect(125, startBtnText.rect.y, 500, 135);
    leaderboardLabel.text = "Leaderboard";

    saveCount = 0;
    leaderboard = getLeaderboard(&saveCount);

    leaderboardFirst.font = font;
    leaderboardFirst.color = createColor("FFD700", 255);
    leaderboardFirst.rect = createRect(leaderboardLabel.rect.x + 25, leaderboardLabel.rect.y + leaderboardLabel.rect.h + 15, 450, 75);
    leaderboardFirst.text = malloc(55 * sizeof(char));
    if (saveCount >= 1) sprintf(leaderboardFirst.text, "1. %s", leaderboard[0]);
    else strcpy(leaderboardFirst.text, "1.          ");

    leaderboardSecond.font = font;
    leaderboardSecond.color = createColor("C0C0C0", 255);
    leaderboardSecond.rect = createRect(leaderboardLabel.rect.x + 25, leaderboardFirst.rect.y + leaderboardFirst.rect.h + 10, 450, 75);
    leaderboardSecond.text = malloc(55 * sizeof(char));
    if (saveCount >= 2) sprintf(leaderboardSecond.text, "2. %s", leaderboard[1]);
    else strcpy(leaderboardSecond.text, "2.          ");

    leaderboardThird.font = font;
    leaderboardThird.color = createColor("CE8946", 255);
    leaderboardThird.rect = createRect(leaderboardLabel.rect.x + 25, leaderboardSecond.rect.y + leaderboardSecond.rect.h + 10, 450, 75);
    leaderboardThird.text = malloc(55 * sizeof(char));
    if (saveCount >= 3) sprintf(leaderboardThird.text, "3. %s", leaderboard[2]);
    else strcpy(leaderboardThird.text, "3.          ");

    leaderboardFourth.font = font;
    leaderboardFourth.color = createColor("FFFFFF", 255);
    leaderboardFourth.rect = createRect(leaderboardLabel.rect.x + 25, leaderboardThird.rect.y + leaderboardThird.rect.h + 10, 450, 75);
    leaderboardFourth.text = malloc(55 * sizeof(char));
    if (saveCount >= 4) sprintf(leaderboardFourth.text, "4. %s", leaderboard[3]);
    else strcpy(leaderboardFourth.text, "4.          ");

    leaderboardFifth.font = font;
    leaderboardFifth.color = createColor("FFFFFF", 255);
    leaderboardFifth.rect = createRect(leaderboardLabel.rect.x + 25, leaderboardFourth.rect.y + leaderboardFourth.rect.h + 10, 450, 75);
    leaderboardFifth.text = malloc(55 * sizeof(char));
    if (saveCount >= 5) sprintf(leaderboardFifth.text, "5. %s", leaderboard[4]);
    else strcpy(leaderboardFifth.text, "5.          ");

    SDL_Color infoColor = createColor("FFFFFF", 255);

    controlsText.font = font;
    controlsText.color = infoColor;
    controlsText.rect = createRect(nicknameLabel.rect.x - 250, nicknameValue.rect.y + 350, 650, 75);
    controlsText.text = "You can sell towers by right clicking on them";

    waterTowerText.font = font;
    waterTowerText.color = infoColor;
    waterTowerText.rect = createRect(controlsText.rect.x + 80, controlsText.rect.y + controlsText.rect.h, 550, 75);
    waterTowerText.text = "- Shoots projectiles at one target";

    crucifixTowerText.font = font;
    crucifixTowerText.color = infoColor;
    crucifixTowerText.rect = createRect(waterTowerText.rect.x, waterTowerText.rect.y + waterTowerText.rect.h, 550, 75);
    crucifixTowerText.text = "- Inflicts area damage over time";

    IncenseTowerText.font = font;
    IncenseTowerText.color = infoColor;
    IncenseTowerText.rect = createRect(crucifixTowerText.rect.x, crucifixTowerText.rect.y + crucifixTowerText.rect.h, 550, 75);
    IncenseTowerText.text = "- Boosts tower damage in an area";

    holyWaterCannonTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/towers/HolyWaterCannon.png");
    incenseTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/towers/IncenseTower.png");
    crucifixTexture = IMG_LoadTexture(renderer, "../scenes/Game/images/towers/CrucifixTower.png");

    waterCannonImage = createRect(waterTowerText.rect.x - 80, waterTowerText.rect.y, 70, 70);
    crucifixImage = createRect(crucifixTowerText.rect.x - 80, crucifixTowerText.rect.y, 70, 70);
    incenseImage = createRect(IncenseTowerText.rect.x - 80, IncenseTowerText.rect.y, 70, 70);
}

void renderMenu(SDL_Renderer* renderer)
{
    SDL_SetTextureColorMod(background, 75, 75, 75);
    SDL_RenderCopy(renderer, background, NULL, &backgroundRect);

    title.color = titleColor;
    startBtnText.color = startColor;
    quitBtnText.color = quitColor;
    highlightButtons();

    if (nickLength >= 5 && searchForButton("nickname")->active) searchForButton("MenuStartBtn")->active = true;
    else {
        searchForButton("MenuStartBtn")->active = false;
        startBtnText.color = createColor("004400", 255);
    }
    
    renderText(renderer, &title);
    renderText(renderer, &startBtnText);
    renderText(renderer, &quitBtnText);
    renderText(renderer, &nicknameLabel);

    renderText(renderer, &leaderboardLabel);
    renderText(renderer, &leaderboardFirst);
    renderText(renderer, &leaderboardSecond);
    renderText(renderer, &leaderboardThird);
    renderText(renderer, &leaderboardFourth);
    renderText(renderer, &leaderboardFifth);

    renderText(renderer, &controlsText);
    renderText(renderer, &waterTowerText);
    renderText(renderer, &crucifixTowerText);
    renderText(renderer, &IncenseTowerText);

    SDL_RenderCopy(renderer, holyWaterCannonTexture, NULL, &waterCannonImage);
    SDL_RenderCopy(renderer, crucifixTexture, NULL, &crucifixImage);
    SDL_RenderCopy(renderer, incenseTexture, NULL, &incenseImage);

    if (!searchForButton("nickname")->active) {
        renderText(renderer, &pressEnterText);
        renderText(renderer, &minCharsText);
    }

    SDL_SetRenderDrawColor(renderer, 25, 25, 25, 255);
    if (isMouseOnRect(nicknameBgButton) && searchForButton("nickname")->active) SDL_SetRenderDrawColor(renderer, 35, 35, 35, 255); 
    SDL_RenderFillRect(renderer, &nicknameBgButton);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    for (size_t i = nickLength; i < 14; i++)
    {
        if (alreadyGotName != true) break;
        nicknameValue.text[i] = ' ';
    }
    
    renderText(renderer, &nicknameValue);
}

void writeSymbol(int sym) {
    alreadyGotName = true;
    if (sym != ' ') nicknameValue.text[nickLength] = sym;
    else nicknameValue.text[nickLength] = '-';
    nickLength++;
}

void delSymbol() {
    nicknameValue.text[nickLength - 1] = '\0';
    nickLength--;
}

void saveNickname() {
    memcpy(player.nick, strtok(nicknameValue.text, " "), strlen(nicknameValue.text) + 1);
}

void freeMenuScene() {
    for (int i = 0; i < saveCount; i++)
    {
        free(leaderboard[i]);
        leaderboard[i] = NULL;
    }
    free(leaderboard);
    leaderboard = NULL;

    free(leaderboardFirst.text);
    free(leaderboardSecond.text);
    free(leaderboardThird.text);
    free(leaderboardFourth.text);
    free(leaderboardFifth.text);

    SDL_DestroyTexture(holyWaterCannonTexture);
    SDL_DestroyTexture(crucifixTexture);
    SDL_DestroyTexture(incenseTexture);
    SDL_DestroyTexture(background);
    
    freeButtons();
    TTF_CloseFont(font);
}