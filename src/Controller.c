//
// Created by johan on 2026-08-29.
//
#include "Controller.h"

#include <joy.h>

void Controller_getState(const u16 port, Controller *controller) {
    const u16 value = JOY_readJoypad(port);
    controller->a = value & BUTTON_A;
    controller->b = value & BUTTON_B;
    controller->c = value & BUTTON_C;
    controller->up = value & BUTTON_UP;
    controller->down = value & BUTTON_DOWN;
    controller->left = value & BUTTON_LEFT;
    controller->right = value & BUTTON_RIGHT;
    controller->start = value & BUTTON_START;
}
