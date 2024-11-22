#include "player.h"

float hp = 500;
int money;

void damagePlayer(float damage) {
    hp -= damage;
}

void addMoney(int amount) {
    money += amount;
}