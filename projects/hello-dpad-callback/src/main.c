#include <genesis.h>

static void handle_input(const u16 controller, const u16 changed, const u16 state) {
    if (changed && state != 0) {
        switch (controller) {
            case JOY_1: {
                if (state & BUTTON_START) {
                    VDP_drawText("Controller 1 Start", 2, 2);
                }
                if (state & BUTTON_A) {
                    VDP_drawText("Controller 1 A", 2, 2);
                }
                if (state & BUTTON_B) {
                    VDP_drawText("Controller 1 B", 2, 2);
                }
                if (state & BUTTON_C) {
                    VDP_drawText("Controller 1 C", 2, 2);
                }
                if (state & BUTTON_UP) {
                    VDP_drawText("Controller 1 UP", 2, 2);
                } else if (state & BUTTON_DOWN) {
                    VDP_drawText("Controller 1 DOWN", 2, 2);
                }
                if (state & BUTTON_LEFT) {
                    VDP_drawText("Controller 1 LEFT", 2, 2);
                } else if (state & BUTTON_RIGHT) {
                    VDP_drawText("Controller 1 RIGHT", 2, 2);
                }
                break;
            }
            case JOY_2: {
                if (state & BUTTON_START) {
                    VDP_drawText(" Controller 2 Start", 2, 2);
                }
                if (state & BUTTON_A) {
                    VDP_drawText(" Controller 2 A", 2, 2);
                }
                if (state & BUTTON_B) {
                    VDP_drawText(" Controller 2 B", 2, 2);
                }
                if (state & BUTTON_C) {
                    VDP_drawText(" Controller 2 C", 2, 2);
                }
                if (state & BUTTON_UP) {
                    VDP_drawText(" Controller 2 UP", 2, 2);
                } else if (state & BUTTON_DOWN) {
                    VDP_drawText(" Controller 2 DOWN", 2, 2);
                }
                if (state & BUTTON_LEFT) {
                    VDP_drawText(" Controller 2 LEFT", 2, 2);
                } else if (state & BUTTON_RIGHT) {
                    VDP_drawText(" Controller 2 RIGHT", 2, 2);
                }
                break;
            }
            default:
                break;
        }
    } else {
        VDP_clearText(2, 2, 32);
    }
}

int main() {
    JOY_init();
    JOY_setEventHandler(handle_input);

    while (TRUE) {
        SYS_doVBlankProcess();
    }
}
