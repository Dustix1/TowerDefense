#pragma once

typedef struct {
    float hp;
    int money;
    int score;
} Player;

extern Player player;

void damagePlayer(float damage);
void addMoney(int amount);