//
// Created by johan on 2026-11-07.
//

#ifndef GENESIS_DOES_WORLD_H
#define GENESIS_DOES_WORLD_H

#include <genesis.h>
#include "Player.h"

extern Player player;

void World_init();
void World_startLevel(u16 level);
void World_switchLevel(u16 level);

#endif //GENESIS_DOES_WORLD_H
