#include <genesis.h>

#include "Controller.h"

static void handle_input(const Controller *controller) {
    VDP_clearText(2, 2, 64);
    if (controller->start) {
        VDP_drawText("Controller 1 Start", 2, 2);
    }
    if (controller->a) {
        VDP_drawText("Controller 1 A", 2, 2);
    }
    if (controller->b) {
        VDP_drawText("Controller 1 B", 2, 2);
    }
    if (controller->c) {
        VDP_drawText("Controller 1 C", 2, 2);
    }
    if (controller->up) {
        VDP_drawText("Controller 1 UP", 2, 2);
    } else if (controller->down) {
        VDP_drawText("Controller 1 DOWN", 2, 2);
    }
    if (controller->left) {
        VDP_drawText("Controller 1 LEFT", 2, 2);
    } else if (controller->right) {
        VDP_drawText("Controller 1 RIGHT", 2, 2);
    }
}

int main(bool b) {
    Controller controller;
    JOY_init();

    while (TRUE) {
        Controller_getState(JOY_1, &controller);
        handle_input(&controller);
        SYS_doVBlankProcess();
    }
}
