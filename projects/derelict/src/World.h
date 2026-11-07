//
// Created by johan on 2026-11-07.
//

#ifndef GENESIS_DOES_WORLD_H
#define GENESIS_DOES_WORLD_H
#include <genesis.h>

#define N_LEVELS 1

void World_init();
void World_startLevel(u16 level);
u16 World_getCurrentLevel();

#endif //GENESIS_DOES_WORLD_H
