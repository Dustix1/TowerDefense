#pragma once

typedef struct {
    float hp;
    int money;
} Player;

extern Player player;

void damagePlayer(float damage);
void addMoney(int amount);