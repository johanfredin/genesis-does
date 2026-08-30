//
// Created by johan on 2026-08-29.
//

#ifndef GENESIS_DOES_CONTROLLER_H
#define GENESIS_DOES_CONTROLLER_H
#include <types.h>

typedef struct Controller_ {
    bool up: 1;
    bool down: 1;
    bool left: 1;
    bool right: 1;
    bool start: 1;
    bool a: 1;
    bool b: 1;
    bool c: 1;
} Controller;

void Controller_getState(u16 port, Controller *controller);
bool Controller_changed(const Controller *controller);
bool Controller_dpadChanged(const Controller *controller);

#endif //GENESIS_DOES_CONTROLLER_H
