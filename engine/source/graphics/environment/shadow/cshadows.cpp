/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "cshadows.h"
void cshadows::init (void) {
    shadows.init();
}

int cshadows::add (ccontext *context, const char *label) {
    sshadow *shadow;
    int result = EE_OK;
    if ((shadow = (sshadow *) emalloc(sizeof(sshadow)))) {
        shadow->context = context;
        shadow->convexhull.init();
        shadow->shape.init();
        result = shadows.add(shadow, label);
    } else
        ekill("out of memory");
    return result;
}

int cshadows::vertex (const char *label, spoint coordinate) {
    sshadow *shadow;
    spoint *point;
    int result = EE_OK;
    if ((shadow = shadows.get(label))) {
        if ((point = (spoint *) emalloc(sizeof(spoint)))) {
            point->positionx = coordinate.positionx;
            point->positiony = coordinate.positiony;
            result = shadow->convexhull.add(point);
        } else
            ekill("out of memory");
    } else
        result = EE_ERROR;
    return result;
}

void cshadows::del (const char *label) {
    sshadow *shadow;
    if ((shadow = shadows.get(label))) {
        destroy(&shadow->convexhull, efree, spoint);
        destroy(&shadow->shape, efree, ssilhouette);
        efree(shadow);
    }
}

int cshadows::dot (spoint left, spoint right) {
    return ((left.positionx*right.positionx)+(left.positiony*right.positiony));
}

spoint cshadows::project (spoint point, spoint light) {
    spoint difference, result;
    difference.positionx = (light.positionx-point.positionx)*(-1);
    difference.positiony = (light.positiony-point.positiony)*(-1);
    result.positionx = point.positionx+difference.positionx;
    result.positiony = point.positiony+difference.positiony;
    return result;
}

void cshadows::recalculate (sshadow *shadow, spoint light, unsigned int intensity, float base, spoint source, spoint destination) {
    spoint midpoint, normal, spot;
    ssilhouette *singleton;
    float distance;
    source.positionx += shadow->context->destination(EPOSITION_POSITIONX);
    source.positiony += shadow->context->destination(EPOSITION_POSITIONY);
    destination.positionx += shadow->context->destination(EPOSITION_POSITIONX);
    destination.positiony += shadow->context->destination(EPOSITION_POSITIONY);
    midpoint.positionx = ((float)(source.positionx+destination.positionx)/2.0f);
    midpoint.positiony = ((float)(source.positiony+destination.positiony)/2.0f);
    normal.positionx = -(source.positiony-destination.positiony);
    normal.positiony = (source.positionx-destination.positionx);
    spot.positionx = (light.positionx-midpoint.positionx);
    spot.positiony = (light.positiony-midpoint.positiony);
    if (cshadows::dot(normal, spot) >= 0) {
        spot.positionx += midpoint.positionx;
        spot.positiony += midpoint.positiony;
        if ((singleton = (ssilhouette *) emalloc(sizeof(ssilhouette)))) {
            singleton->source = source;
            singleton->destination = destination;
            singleton->projection[0] = cshadows::project(source, light);
            singleton->projection[1] = cshadows::project(destination, light);
            distance = ccollision::distance(source, spot);
            singleton->alpha[0] = ((base*(float)EE_MAX(((int)intensity-distance),0))/(float)intensity);
            distance = ccollision::distance(destination, spot);
            singleton->alpha[1] = ((base*(float)EE_MAX(((int)intensity-distance),0))/(float)intensity);
            shadow->shape.add(singleton);
        } else
            ekill("out of memory");
    }
}

void cshadows::recalculate (sshadow *shadow, spoint light, unsigned int intensity, float base) {
    spoint *singleton[EE_SEGMENTS] = {NULL, NULL};
    spoint normalized[EE_SEGMENTS], pivot;
    float sina, cosa;
    int position, index;
    destroy(&shadow->shape, efree, ssilhouette);
    pivot = ccollision::pivot(shadow->context);
    pivot.positionx -= shadow->context->destination(EPOSITION_POSITIONX);
    pivot.positiony -= shadow->context->destination(EPOSITION_POSITIONY);
    position = (int)(fmodf(shadow->context->rotation, 360.0)*10.0f);
    sina = cmath::esina[position];
    cosa = cmath::ecosa[position];
    for (index = 0; index < shadow->convexhull.records(); index++)
        if ((singleton[0] = shadow->convexhull.get((unsigned int)index)))
            if ((singleton[1] = shadow->convexhull.get((unsigned int)((index+1)%shadow->convexhull.records())))) {
                normalized[EE_SEGMENT_ONE] = ccollision::rotate(*singleton[EE_SEGMENT_ONE], pivot, sina, cosa);
                normalized[EE_SEGMENT_TWO] = ccollision::rotate(*singleton[EE_SEGMENT_TWO], pivot, sina, cosa);
                cshadows::recalculate(shadow, light, intensity, base, normalized[EE_SEGMENT_ONE], normalized[EE_SEGMENT_TWO]);
            }
}

void cshadows::unload (sshadow *singleton) {
    destroy(&singleton->convexhull, efree, spoint);
    destroy(&singleton->shape, efree, ssilhouette);
    efree(singleton);
}

void cshadows::unload (void) {
    destroy(&shadows, this->unload, sshadow);
}
