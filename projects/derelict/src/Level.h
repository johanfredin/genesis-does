//
// Created by johan on 2026-11-07.
//

#ifndef GENESIS_DOES_LEVEL_H
#define GENESIS_DOES_LEVEL_H
#include <genesis.h>

#include "LevelDefs.h"

typedef struct Level_ {
    Vect2D_u16 spawn;
    Vect2D_u16 bounds;
    const TileMap *collisionTiles;
    const Image *bg;
    const Map *map;

    /*
     * TODO: Implement:
     * enemies
     * pickups
     * goal
     */
} Level;

void Level_init(Level *level, const LevelDefinition *def, u16 tileIndex);
void Level_update(Level *level);
void Level_render(Level *level);
void Level_destroy(Level *level);

#endif //GENESIS_DOES_LEVEL_H
