#ifndef GAMESTATUS_H
#define GAMESTATUS_H

struct {
    int currentScene;
} gameStatus;

void changeScene(int scene);

#endif