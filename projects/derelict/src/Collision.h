//
// Created by johan on 2026-10-03.
//

#ifndef GENESIS_DOES_COLLISION_H
#define GENESIS_DOES_COLLISION_H

#include <genesis.h>

typedef struct CollisionBox {
    ff32 x, y;
    ff32 w, h;
} CollisionBox;

static inline bool Collision_detect(const CollisionBox *a, const CollisionBox *b) {
    return a->x < (b->x + b->w) &&
           (a->x + a->w) > b->x &&
           a->y < (b->y + b->h) &&
           (a->y + a->h) > b->y;
}

Vect2D_ff32 Collision_resolveStatic(const CollisionBox *curr, const CollisionBox *prev, const CollisionBox *b);

void Collision_resolveEntityVsBox(const CollisionBox *a, Vect2D_ff32 aPrev, CollisionBox *b);
void Collision_resolveEntityVsEntity(CollisionBox *a, Vect2D_ff32 aPrev, CollisionBox *b, Vect2D_ff32 bPrev);

#endif //GENESIS_DOES_COLLISION_H
