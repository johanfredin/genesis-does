//
// Created by johan on 2026-11-07.
//

#include "World.h"

#include <genesis.h>

#include "Level.h"

static u16 currentLevel = 0;

void World_init() {
    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
    VDP_setWindowVPos(FALSE, 2);
}

void World_startLevel(const u16 level) {
    currentLevel = level;
}

u16 World_getCurrentLevel() {
    return currentLevel;
}
