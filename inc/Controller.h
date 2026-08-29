//
// Created by johan on 2026-08-29.
//

#ifndef GENESIS_DOES_CONTROLLER_H
#define GENESIS_DOES_CONTROLLER_H
#include <types.h>

typedef struct Controller_ {
    u8 up: 1;
    u8 down: 1;
    u8 left: 1;
    u8 right: 1;
    u8 start: 1;
    u8 a: 1;
    u8 b: 1;
    u8 c: 1;
} Controller;

void Controller_getState(u16 port, Controller *controller);

#endif //GENESIS_DOES_CONTROLLER_H
