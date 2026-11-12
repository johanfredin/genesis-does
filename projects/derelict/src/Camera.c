//
// Created by johan on 2026-11-11.
//

#include "Camera.h"

#include "Game.h"
#include "Log.h"

typedef struct Cam_ {
    Vect2D_s16 pos;
    Vect2D_s16 viewPortSize;
} Cam;

static Cam cam = {0};

void Camera_init(const Vect2D_s16 pos, const Vect2D_s16 viewportSize) {
    cam.pos = pos;
    cam.viewPortSize = viewportSize;
    LOG_debug("Camera initialized");
}

void Camera_update(const Vect2D_ff32 pos) {
    cam.pos.x = (s16) (FF32_toInt(pos.x) - (SCREEN_WIDTH >> 1));
    cam.pos.y = (s16) (FF32_toInt(pos.y) - (SCREEN_HEIGHT >> 1));

    if (cam.pos.x < 0) {
        cam.pos.x = 0;
    } else if (cam.pos.x >= cam.viewPortSize.x - SCREEN_WIDTH) {
        cam.pos.x = (s16) (cam.viewPortSize.x - SCREEN_WIDTH);
    }
    if (cam.pos.y < 0) {
        cam.pos.y = 0;
    } else if (cam.pos.y >= cam.viewPortSize.y - SCREEN_HEIGHT) {
        cam.pos.y = (s16) (cam.viewPortSize.y - SCREEN_HEIGHT);
    }
}

const Vect2D_s16 *Camera_getPos() {
    return &cam.pos;
}
