#include "physics.h"

#include "player.h"

#define CORE_ACCEL_SCALE    0.15f
#define CORE_FRICTION       0.90f
#define CORE_MAX_SPEED      0.1f

void physics_update(core_t *core, physics_t *physics)
{
    core->vx += physics->ax * CORE_ACCEL_SCALE;
    core->vy += physics->ay * CORE_ACCEL_SCALE;

    core->vx *= CORE_FRICTION;
    core->vy *= CORE_FRICTION;

    if (core->vx > CORE_MAX_SPEED)
        core->vx = CORE_MAX_SPEED;

    if (core->vx < -CORE_MAX_SPEED)
        core->vx = -CORE_MAX_SPEED;

    if (core->vy > CORE_MAX_SPEED)
        core->vy = CORE_MAX_SPEED;

    if (core->vy < -CORE_MAX_SPEED)
        core->vy = -CORE_MAX_SPEED;

    core->x += core->vx;
    core->y += core->vy;


    
}