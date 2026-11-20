//
// Created by johan on 2026-11-07.
//

#include "MainMenu.h"
#include "Log.h"
#include "World.h"

void MainMenu_init() {
    //TODO: Implement
    LOG_info("Main menu initialized");
}

void MainMenu_render() {}
void MainMenu_update() {}

void MainMenu_startGame() {
    //TODO: Implement proper
    World_startLevel(0);
    LOG_debug("Level 0 started");
}