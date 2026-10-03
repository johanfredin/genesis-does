//
// Created by johan on 2026-10-03.
//

#include "Collision.h"

Vect2D_ff32 Collision_resolveStatic(const CollisionBox *curr, const CollisionBox *prev, const CollisionBox *b) {
    Vect2D_ff32 push = {0};

    // Horizontal collision
    if ((prev->x + prev->w) <= b->x) {              // From left
        push.x = b->x - (curr->x + curr->w);
    } else if (prev->x >= (b->x + b->w)) {          // From right
        push.x = (b->x - b->x) - curr->x;
    }

    // Vertical collision
    if ((prev->y + prev->h) <= b->y) {              // From above
        push.y = b->y - (curr->y + curr->h);
    } else if (prev->y >= (b->y + b->h)) {          // From below
        push.y = (b->y + b ->h) - curr->y;
    }

    return push;
}

void Collision_resolveEntityVsBox(const CollisionBox *a, Vect2D_ff32 aPrev, CollisionBox *b) {

}
void Collision_resolveEntityVsEntity(CollisionBox *a, Vect2D_ff32 aPrev, CollisionBox *b, Vect2D_ff32 bPrev) {}
