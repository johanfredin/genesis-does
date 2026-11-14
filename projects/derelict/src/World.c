//
// Created by johan on 2026-11-07.
//

#include "World.h"
#include "Level.h"
#include "../res/resources.h"

static u16 tileIndex = TILE_USER_INDEX;
static u16 hudTiles = 0;
static Level currentLevel = {0};
static Player player = {0};

void World_init() {
    //TODO: Load HUD
    tileIndex += hudTiles;

    // Init player ---------------------------------------------------------
    PAL_setPalette(PAL2, spr_cat.palette->data, DMA);
    Player_init(
        &player,
        &spr_cat,
        TILE_ATTR(PAL2, 0, 0, 0),
        (Vect2D_s16) {.x = 170, .y = 190},
        16,
        16,
        16,
        32
    );

    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
    VDP_setWindowVPos(FALSE, 2);
}

void World_startLevel(const u16 level) {
    const LevelDefinition *def = LevelDefs_get(level);
    Level_init(&currentLevel, def, &tileIndex, &player);
}

void World_switchLevel(const u16 level) {
    Level_destroy(&currentLevel);
    tileIndex = TILE_USER_INDEX + hudTiles;
    World_startLevel(level);
}
