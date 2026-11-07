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
void MainMenu_startGame() {
    //TODO: Implement proper
    World_startLevel(1);
    LOG_debug("Game started");
}