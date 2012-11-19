/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "cimpacts.h"
void cimpacts::init (void) {
    colliders.init();
}

int cimpacts::add (ccontext *context, const char *label, fimpacthandler *handler) {
    scollider *collider;
    int result = EE_OK;
    if ((collider = (scollider *) emalloc(sizeof(scollider)))) {
        collider->handler = handler;
        if ((collider->agent = (sagent *) emalloc(sizeof(sagent)))) {
            collider->agent->informations = NULL;
            collider->agent->group = EE_IMPACT_NOGROUP;
            collider->agent->context = context;
            collider->cover.offset.positionx = collider->cover.offset.positiony = 0;
            collider->cover.width = context->destination(EPOSITION_WIDTH);
            collider->cover.height = context->destination(EPOSITION_HEIGHT);
            collider->boxes.init();
            result = colliders.add(collider, label);
        } else
            ekill("out of memory");
    } else
        ekill("out of memory");
    return result;
}

int cimpacts::box (const char *label, spoint offset, unsigned int width, unsigned int height) {
    scollider *singleton;
    sarea *area;
    int result = EE_OK;
    if ((singleton = colliders.get(label))) {
        if ((area = (sarea *) emalloc(sizeof(sarea)))) {
            area->offset = offset;
            area->width = width;
            area->height = height;
            result = singleton->boxes.add(area);
        } else
            ekill("out of memory");
    } else
        result = EE_ERROR;
    return result;
}

int cimpacts::cover (const char *label, spoint offset, unsigned int width, unsigned int height) {
    scollider *singleton;
    int result = EE_OK;
    if ((singleton = colliders.get(label))) {
        singleton->cover.offset = offset;
        singleton->cover.width = width;
        singleton->cover.height = height;
    } else
        result = EE_ERROR;
    return result;
}

sagent *cimpacts::get (const char *label) {
    scollider *singleton;
    sagent *result = NULL;
    if ((singleton = colliders.get(label)))
        result = singleton->agent;
    return result;
}

void cimpacts::del (const char *label) {
    scollider *collider;
    if ((collider = colliders.get(label))) {
        if (collider->agent)
            efree(collider->agent);
        destroy(&collider->boxes, efree, sarea);
        efree(collider);
        colliders.del();
    }
}

void cimpacts::recalculate (void) {
    scollider *collider, *agent;
    int index, subindex, positionx[EE_RECTANGLES], positiony[EE_RECTANGLES];
    scollision impact;
    srectangle cover[EE_RECTANGLES];
    sarea *box[EE_RECTANGLES];
    for (index = 0; index < colliders.records(); index++)
        if ((agent = colliders.get(index)))
            for (subindex = (index+1); subindex < colliders.records(); subindex++)
                if ((collider = colliders.get(subindex))) {
                    if ((collider->agent->group == EE_IMPACT_NOGROUP) || (collider->agent->group != agent->agent->group)) {
                        positionx[EE_RECTANGLE_ONE] = agent->agent->context->destination(EPOSITION_POSITIONX);
                        positiony[EE_RECTANGLE_ONE] = agent->agent->context->destination(EPOSITION_POSITIONY);
                        cover[EE_RECTANGLE_ONE].angle = agent->agent->context->rotation;
                        cover[EE_RECTANGLE_ONE].origin.positionx = positionx[EE_RECTANGLE_ONE]+agent->cover.offset.positionx;
                        cover[EE_RECTANGLE_ONE].origin.positiony = positiony[EE_RECTANGLE_ONE]+agent->cover.offset.positiony;
                        cover[EE_RECTANGLE_ONE].width = agent->cover.width;
                        cover[EE_RECTANGLE_ONE].height = agent->cover.height;
                        cover[EE_RECTANGLE_ONE].pivot = ccollision::pivot(agent->agent->context);
                        positionx[EE_RECTANGLE_TWO] = collider->agent->context->destination(EPOSITION_POSITIONX);
                        positiony[EE_RECTANGLE_TWO] = collider->agent->context->destination(EPOSITION_POSITIONY);
                        cover[EE_RECTANGLE_TWO].angle = collider->agent->context->rotation;
                        cover[EE_RECTANGLE_TWO].origin.positionx = positionx[EE_RECTANGLE_TWO]+collider->cover.offset.positionx;
                        cover[EE_RECTANGLE_TWO].origin.positiony = positiony[EE_RECTANGLE_TWO]+collider->cover.offset.positiony;
                        cover[EE_RECTANGLE_TWO].width = collider->cover.width;
                        cover[EE_RECTANGLE_TWO].height = collider->cover.height;
                        cover[EE_RECTANGLE_TWO].pivot = ccollision::pivot(collider->agent->context);
                        impact = ccollision::collision(cover[EE_RECTANGLE_ONE], cover[EE_RECTANGLE_TWO]);
                        if (impact.collision) {
                            /* before search if a box is inside another */
                            foreach(box[EE_RECTANGLE_ONE], &agent->boxes) {
                                foreach(box[EE_RECTANGLE_TWO], &collider->boxes) {
                                    cover[EE_RECTANGLE_ONE].origin.positionx = positionx[EE_RECTANGLE_ONE]+box[EE_RECTANGLE_ONE]->offset.positionx;
                                    cover[EE_RECTANGLE_ONE].origin.positiony = positiony[EE_RECTANGLE_ONE]+box[EE_RECTANGLE_ONE]->offset.positiony;
                                    cover[EE_RECTANGLE_ONE].width = box[EE_RECTANGLE_ONE]->width;
                                    cover[EE_RECTANGLE_ONE].height = box[EE_RECTANGLE_ONE]->height;
                                    cover[EE_RECTANGLE_TWO].origin.positionx = positionx[EE_RECTANGLE_TWO]+box[EE_RECTANGLE_TWO]->offset.positionx;
                                    cover[EE_RECTANGLE_TWO].origin.positiony = positiony[EE_RECTANGLE_TWO]+box[EE_RECTANGLE_TWO]->offset.positiony;
                                    cover[EE_RECTANGLE_TWO].width = box[EE_RECTANGLE_TWO]->width;
                                    cover[EE_RECTANGLE_TWO].height = box[EE_RECTANGLE_TWO]->height;
                                    
                                    impact = ccollision::collision(cover[EE_RECTANGLE_ONE], cover[EE_RECTANGLE_TWO]);
                                    if (impact.collision) {
                                        if (agent->handler)
                                            agent->handler(collider->agent, agent->agent, impact.impact);
                                        if (collider->handler)
                                            collider->handler(agent->agent, collider->agent, impact.impact);
                                    }
                                }
                            }
                        }
                    }
                }
}

void cimpacts::unload (scollider *singleton) {
    if (singleton->agent)
        efree(singleton->agent);
    destroy(&singleton->boxes, efree, sarea);
    efree(singleton);
}

void cimpacts::unload (void) {
    destroy(&colliders, this->unload, scollider);
}