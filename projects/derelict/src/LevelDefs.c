//
// Created by johan on 2026-11-07.
//

#include "LevelDefs.h"

#include "../res/resources.h"

#define N_LEVELS 1

static const LevelDefinition levels[N_LEVELS] = {
    {
        // Level 1
        .paletteBG = &palette_bg_jungle3,
        .paletteFG = &palette_fg_jungle3,
        .tilesetBG = &map_bg_jungle3_tileset0,
        .tilesetFG = &map_fg_jungle3_tileset0,
        .mapBg = &map_bg_jungle3,
        .mapFg = &map_fg_jungle3,
        .collision = &tilemap_collision
    }
};

const LevelDefinition *LevelDefs_get(const u16 n) {
    //TODO: Add debug assert that we are within bounds
    return &levels[n];
}