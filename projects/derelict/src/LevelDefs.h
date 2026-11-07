//
// Created by johan on 2026-11-07.
//

#ifndef GENESIS_DOES_LEVELDEFS_H
#define GENESIS_DOES_LEVELDEFS_H

#include <genesis.h>

typedef struct LevelDef_ {
    const Image *bg;                    // BG_B, palette in bg->palette
    const MapDefinition *map;           // BG_A
    const Palette *mapPalette;
    const TileSet *const *mapTilesets;
    const TileMap *collision;
    u8 numMapTilesets;
} LevelDefinition;

const LevelDefinition *LevelDefs_get(u16 n);

#endif //GENESIS_DOES_LEVELDEFS_H
