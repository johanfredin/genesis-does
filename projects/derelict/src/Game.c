//
// Created by johan on 2026-11-07.
//

#include "Game.h"

#include <genesis.h>

#include "MainMenu.h"
#include "Controller.h"
#include "World.h"

Controller controller = {0};
GameState gameState = 0;

static void Game_run();


void Game_init() {
    SPR_initEx(36);
    VDP_setScreenWidth320();
    VDP_setScreenHeight224();

    Controller_init();
    MainMenu_init();
    World_init();
    MainMenu_startGame();


    SYS_doVBlankProcess();

    // TODO: Implement proper main menu
    gameState = RUNNING;

    Game_run();
}

static void Game_run() {
    while (true) {
        switch (gameState) {
            case RUNNING:
                World_update();
                World_render();
                break;
            case MAIN_MENU:
                MainMenu_update();
                MainMenu_render();
                break;
            case PAUSED:
                World_render();
                break;
        }

        SPR_update();
        SYS_doVBlankProcess();
    }
}