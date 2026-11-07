//
// Created by johan on 2026-11-07.
//

#ifndef GENESIS_DOES_LEVEL_H
#define GENESIS_DOES_LEVEL_H
#include <genesis.h>

typedef struct Level_ {
    Vect2D_u16 spawn;
    Vect2D_u16 bounds;
    /*
     * TODO: Implement:
     * enemies
     * pickups
     * goal
     */
} Level;

void Level_init(Level *level);
void Level_update(Level *level);
void Level_render(Level *level);
void Level_destroy(Level *level);

#endif //GENESIS_DOES_LEVEL_H
