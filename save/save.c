#include "save.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/gameStatus.h"
#include "../scenes/Game/Friendly/player.h"

static FILE* saveFile;
static char** saves;

void saveData() {
    saveFile = fopen("../save/players.save", "r");
    if (saveFile == NULL) {
        freeCurrentScene();
        changeScene(NONE);
        gameStatus.running = false;
    }

    char buff[50];
    int saveCount = 0;
    while (fgets(buff, sizeof(buff), saveFile))
    {
        saves = realloc(saves, (saveCount + 1) * sizeof(char*));
        saves[saveCount] = malloc(50 * sizeof(char));
        memcpy(saves[saveCount], buff, strlen(buff) + 1);
        saveCount++;
    }
    fclose(saveFile);
    bool isAlreadyInFile = false;
    for (size_t i = 0; i < saveCount; i++)
    {
        char buff[50];
        strcpy(buff, saves[i]);
        char* savedNick = strtok(buff, " ");
        char* savedScore = strtok(NULL, " ");

        int savedScoreInt = atoi(savedScore);
        if(strcmp(savedNick, player.nick) == 0) {
            if (player.score > savedScoreInt) sprintf(saves[i], "%s %d", player.nick, player.score);
            isAlreadyInFile = true;
        }
    }


    saveFile = fopen("../save/players.save", "w");
    if (saveFile == NULL) {
        freeCurrentScene();
        changeScene(NONE);
        gameStatus.running = false;
    }

    if (saveCount != 0) {
        if (isAlreadyInFile) {
            for (size_t i = 0; i < saveCount; i++)
            {
                strtok(saves[i], "\n");
                fprintf(saveFile, "%s\n", saves[i]);
            }   
        } else {
            for (size_t i = 0; i < saveCount; i++)
            {
                strtok(saves[i], "\n");
                fprintf(saveFile, "%s\n", saves[i]);
            }
            fprintf(saveFile, "%s %d", player.nick, player.score);
        }
    } else {
        fprintf(saveFile, "%s %d", player.nick, player.score);
    }
    fclose(saveFile);

    for (size_t i = 0; i < saveCount; i++)
    {
        free(saves[i]);
        saves[i] = NULL;
    }
    free(saves);
    saves = NULL;
    saveCount = 0;
}