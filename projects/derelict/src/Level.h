//
// Created by johan on 2026-11-07.
//

#ifndef GENESIS_DOES_LEVEL_H
#define GENESIS_DOES_LEVEL_H
#include <genesis.h>

#include "LevelDefs.h"
#include "Player.h"

typedef struct Level_ {
    Vect2D_u16 spawn;
    Vect2D_u16 bounds;
    const TileMap *collisionTiles;
    const Map *bg;
    const Map *fg;

    /*
     * TODO: Implement:
     * enemies
     * pickups
     * goal
     */
} Level;

void Level_init(Level *level, const LevelDefinition *def, u16 *tileIndex, Player *player);
void Level_update(Level *level);
void Level_render();
void Level_destroy(const Level *level);

#endif //GENESIS_DOES_LEVEL_H
