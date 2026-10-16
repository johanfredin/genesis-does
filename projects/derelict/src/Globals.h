//
// Created by johan on 2026-10-03.
//

#ifndef GENESIS_DOES_GLOBALS_H
#define GENESIS_DOES_GLOBALS_H
#include "Controller.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 224
#define MAP_WIDTH 512
#define MAP_HEIGHT 320

static const ff32 GRAVITY = FF32(0.66f);
static const ff32 MAX_GRAVITY = FF32(6.5f);

extern Controller controller;
extern Vect2D_s16 camera;

#endif //GENESIS_DOES_GLOBALS_H
