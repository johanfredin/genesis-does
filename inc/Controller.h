//
// Created by johan on 2026-08-29.
//

#ifndef GENESIS_DOES_CONTROLLER_H
#define GENESIS_DOES_CONTROLLER_H
#include <types.h>
#include <joy.h>

typedef struct Controller_ {
    u16 current;
    u16 previous;
} Controller;

void Controller_init();

static inline u16 Controller_poll(Controller *const controller, const u16 port) {
    controller->previous = controller->current;
    controller->current = JOY_readJoypad(port);
    return controller->current;
}

static inline bool Controller_isHeld(const Controller *const controller, const u16 button) {
    return controller->current & button;
}

static inline bool Controller_wasPressed(const Controller *const controller, const u16 button) {
    return controller->current & button && !(controller->previous & button);
}

static inline bool Controller_wasReleased(const Controller *const controller, const u16 button) {
    return controller->previous & button && !(controller->current & button);
}


#endif //GENESIS_DOES_CONTROLLER_H
