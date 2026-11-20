//
// Created by johan on 2026-11-07.
//

#ifndef GENESIS_DOES_GAME_H
#define GENESIS_DOES_GAME_H

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 224
#include "Controller.h"

typedef enum GameState_: u8 {
    PAUSED,
    MAIN_MENU,
    RUNNING
} GameState;

extern Controller controller;
extern GameState gameState;

void Game_init();

#endif //GENESIS_DOES_GAME_H
