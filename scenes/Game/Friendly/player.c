#include "player.h"

#include "../gameScene.h"

Player player;

void damagePlayer(float damage) {
    player.hp -= damage;
    flashVan();
}

void addMoney(int amount) {
    player.money += amount;
}