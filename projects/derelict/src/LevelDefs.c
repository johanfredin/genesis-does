//
// Created by johan on 2026-11-07.
//

#include "LevelDefs.h"

#include "../res/resources.h"

#define N_LEVELS 1

static const TileSet *const level1TileSets[] = {
    &tileset_fg_jungle3
};

static const LevelDefinition levels[N_LEVELS] = {
    {
        // Level 1
        .bg = nullptr,
        .map = &map_fg_jungle3,
        .mapPalette = &palette_fg_jungle3,
        .mapTilesets = level1TileSets,
        .numMapTilesets = sizeof(level1TileSets) / sizeof(level1TileSets[0]),
        .collision = &tilemap_collision
    }
};

const LevelDefinition *LevelDefs_get(const u16 n) {
    //TODO: Add debug assert that we are within bounds
    return &levels[n];
}