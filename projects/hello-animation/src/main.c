// ReSharper disable CppDFAEndlessLoop
#include <genesis.h>

#include "Controller.h"
#include "../res/resources.h"

#define ANIM_IDLE 0
#define ANIM_WALK 1

#define HEADING_LEFT 0
#define HEADING_RIGHT 1

static Sprite *player = nullptr;
static Controller controller = {0};

static void init() {
    SPR_init();
    PAL_setPalette(PAL2, spr_def_juan.palette->data, DMA);
    JOY_init();
    player = SPR_addSprite(
        &spr_def_juan,
        (s16)((VDP_getScreenWidth() >> 1) - (spr_def_juan.w >> 1)),
        (s16)((VDP_getScreenHeight() >> 1) - (spr_def_juan.h >> 1)),
        TILE_ATTR(PAL2, 1, 0, 0)
    );

    VDP_drawText("Hello animated sprite!", 1, 1);
}

static void update() {
    static s16 anim = 0;
    static const fix16 vel = FIX16(3.5f);
    fix16 x = FIX16(SPR_getPositionX(player));
    fix16 y = FIX16(SPR_getPositionY(player));
    if (Controller_poll(&controller, JOY_1)) {
        if (Controller_isHeld(&controller, BUTTON_UP)) {
            y -= vel;
        } else if (Controller_isHeld(&controller, BUTTON_DOWN)) {
            y += vel;
        }
        if (Controller_isHeld(&controller, BUTTON_LEFT)) {
            x -= vel;
            SPR_setHFlip(player, true);
        } else if (Controller_isHeld(&controller, BUTTON_RIGHT)) {
            x += vel;
            SPR_setHFlip(player, false);
        }
        anim = ANIM_WALK;
        SPR_setPosition(player, F16_toInt(x), F16_toInt(y));
    } else {
        anim = ANIM_IDLE;
    }

    SPR_setAnim(player, anim);
    SPR_update();
}

int main(bool bool_) {
    init();
    while (TRUE) {
        update();
        SYS_doVBlankProcess();
    }

}
