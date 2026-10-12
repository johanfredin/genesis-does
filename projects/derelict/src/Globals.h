//
// Created by johan on 2026-10-03.
//

#ifndef GENESIS_DOES_GLOBALS_H
#define GENESIS_DOES_GLOBALS_H
#include "Controller.h"

typedef struct TMX_HardBlock {
    u16 x, y, w, h;
} TMX_HardBlock;

typedef struct TMX_Platform {
    u16 x, y, w, h;
} TMX_Platform;

extern Controller controller;


#endif //GENESIS_DOES_GLOBALS_H
