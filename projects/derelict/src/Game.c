//
// Created by johan on 2026-11-07.
//

#include "Game.h"

#include <genesis.h>

#include "MainMenu.h"
#include "Controller.h"
#include "World.h"

Controller controller = {0};

void Game_init() {
    SPR_initEx(36);
    VDP_setScreenWidth320();
    VDP_setScreenHeight224();

    Controller_init();
    MainMenu_init();
    World_init();

    SYS_doVBlankProcess();
}