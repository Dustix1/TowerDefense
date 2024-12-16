#include "player.h"

Player player;

void damagePlayer(float damage) {
    player.hp -= damage;
}

void addMoney(int amount) {
    player.money += amount;
}