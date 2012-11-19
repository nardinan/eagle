/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "cpaths.h"
void cpaths::init (void) {
    movements.init();
}

int cpaths::add (ccontext *context, const char *label) {
    smovement *movement;
    int result = EE_OK;
    if ((movement = (smovement *) emalloc(sizeof(smovement)))) {
        movement->context = context;
        movement->paths.init();
        movement->stopwatch.init();
        movement->stopwatch.add("d");
        movement->stopwatch.add("x");
        movement->stopwatch.add("y");
        result = movements.add(movement, label);
    } else
        ekill("out of memory");
    return result;
}

int cpaths::add (unsigned int positionx, unsigned int positiony, unsigned int running, const char *label, bool destroy) {
    smovement *movement;
    spath *path;
    int result = EE_OK;
    if ((movement = movements.get(label))) {
        if ((path = (spath *) emalloc(sizeof(spath)))) {
            path->destination.positionx = positionx;
            path->destination.positiony = positiony;
            path->running = running;
            path->destroy = destroy;
            path->executing = false;
            path->update[EPOSITION_POSITIONX] = path->update[EPOSITION_POSITIONY] = 0;
            result = movement->paths.add(path);
        } else
            ekill("out of memory");
    } else
        result = EE_ERROR;
    return result;
}

void cpaths::del (const char *label) {
    smovement *movement;
    if ((movement = movements.get(label))) {
        destroy(&movement->paths, efree, spath);
        movement->stopwatch.unload();
        efree(movement);
        movements.del();
    }
}

int cpaths::fflush (const char *label) {
    smovement *movement;
    int result = EE_OK;
    if ((movement = movements.get(label)))
        destroy(&movement->paths, efree, spath);
    else
        result = EE_ERROR;
    return result;
}

void cpaths::recalculate (void) {
    smovement *movement;
    spath *path;
    unsigned int remaining, distance;
    foreach(movement, &movements) {
        if ((path = movement->paths.get((size_t)0))) {
            if (path->executing) {
                if ((remaining = EE_MAX(((int)path->running-(int)movement->stopwatch.get("d").usecs), 0)) > 0) {
                    if ((distance = abs(path->destination.positionx-movement->context->destination(EPOSITION_POSITIONX))) > 0)
                        path->update[EPOSITION_POSITIONX] = remaining/(distance/EE_PATHS_MOVEMENT);
                    if ((distance = abs(path->destination.positiony-movement->context->destination(EPOSITION_POSITIONY))) > 0)
                        path->update[EPOSITION_POSITIONY] = remaining/(distance/EE_PATHS_MOVEMENT);
                    if (movement->stopwatch.get("x").usecs >= path->update[EPOSITION_POSITIONX]) {
                        if (path->destination.positionx > movement->context->destination(EPOSITION_POSITIONX))
                            movement->context->drop(EPOSITION_POSITIONX, (movement->context->destination(EPOSITION_POSITIONX)+EE_PATHS_MOVEMENT));
                        if (path->destination.positionx < movement->context->destination(EPOSITION_POSITIONX))
                            movement->context->drop(EPOSITION_POSITIONX, (movement->context->destination(EPOSITION_POSITIONX)-EE_PATHS_MOVEMENT));
                        movement->stopwatch.set("x");
                    }
                    if (movement->stopwatch.get("y").usecs >= path->update[EPOSITION_POSITIONY]) {
                        if (path->destination.positiony > movement->context->destination(EPOSITION_POSITIONY))
                            movement->context->drop(EPOSITION_POSITIONY, (movement->context->destination(EPOSITION_POSITIONY)+EE_PATHS_MOVEMENT));
                        if (path->destination.positiony < movement->context->destination(EPOSITION_POSITIONY))
                            movement->context->drop(EPOSITION_POSITIONY, (movement->context->destination(EPOSITION_POSITIONY)-EE_PATHS_MOVEMENT));
                        movement->stopwatch.set("y");
                    }
                } else {
                    movement->context->drop(EPOSITION_POSITIONX, path->destination.positionx);
                    movement->context->drop(EPOSITION_POSITIONY, path->destination.positiony);
                    movement->paths.del();
                    if (!path->destroy) {
                        path->executing = false;
                        movement->paths.add(path);
                    } else
                        efree(path);
                }
            } else {
                movement->stopwatch.set("d");
                movement->stopwatch.set("x");
                movement->stopwatch.set("y");
                path->executing = true;
            }
        }
    }
}

void cpaths::unload (smovement *singleton) {
    destroy(&singleton->paths, efree, spath);
    singleton->stopwatch.unload();
    efree(singleton);
}

void cpaths::unload (void) {
    destroy(&movements, this->unload, smovement);
}
