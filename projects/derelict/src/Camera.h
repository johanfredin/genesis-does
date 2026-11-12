//
// Created by johan on 2026-11-11.
//

#ifndef GENESIS_DOES_CAMERA_H
#define GENESIS_DOES_CAMERA_H
#include <genesis.h>

void Camera_init(Vect2D_s16 pos, Vect2D_s16 viewportSize);
void Camera_update(Vect2D_ff32 pos);
const Vect2D_s16 *Camera_getPos();

#endif //GENESIS_DOES_CAMERA_H
