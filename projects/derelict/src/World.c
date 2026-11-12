//
// Created by johan on 2026-11-07.
//

#include "World.h"

#include <genesis.h>

#include "Level.h"

static u16 tileIndex = TILE_USER_INDEX;
static u16 hudTiles = 0;
static Level currentLevel = {0};

Player player = {0};

void World_init() {
    //TODO: Load HUD
    tileIndex += hudTiles;

    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
    VDP_setWindowVPos(FALSE, 2);
}

void World_startLevel(const u16 level) {
    const LevelDefinition *def = LevelDefs_get(level);
    Level_init(&currentLevel, def, &tileIndex);
}

void World_switchLevel(const u16 level) {
    Level_destroy(&currentLevel);
    tileIndex = TILE_USER_INDEX + hudTiles;
    World_startLevel(level);
}
