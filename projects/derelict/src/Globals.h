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

static inline  CollisionBox tmx_toWorldBox(const TMX_HardBlock *block) {
    return (CollisionBox){
        FF32_fromInt(block->x),
        FF32_fromInt(block->y),
        FF32_fromInt(block->w),
        FF32_fromInt(block->h)
    };
}

extern Controller controller;


#endif //GENESIS_DOES_GLOBALS_H
