//
// Created by johan on 2026-11-07.
//

#ifndef GENESIS_DOES_LEVELDEFS_H
#define GENESIS_DOES_LEVELDEFS_H

#include <genesis.h>

typedef struct LevelDef_ {
    const TileSet *tilesetBG;
    const TileSet *tilesetFG;
    const Palette *paletteBG;
    const Palette *paletteFG;
    const MapDefinition *mapBg;
    const MapDefinition *mapFg;
    const TileMap *collision;
} LevelDefinition;

const LevelDefinition *LevelDefs_get(u16 n);

#endif //GENESIS_DOES_LEVELDEFS_H
