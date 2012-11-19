/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "ccollision.h"
scollision ccollision::collision (srectangle collider, srectangle agent) {
    spoint center[EE_RECTANGLES];
    sline definition[EE_RECTANGLES][EAXIS_NULL];
    float sina[EE_RECTANGLES], cosa[EE_RECTANGLES];
    int position[EE_RECTANGLES], significant, axis, subaxis;
    scollision result;
    result.collision = false;
    center[EE_RECTANGLE_ONE] = ccollision::pivot(collider.origin.positionx, collider.origin.positiony, collider.width, collider.height);
    center[EE_RECTANGLE_TWO] = ccollision::pivot(agent.origin.positionx, agent.origin.positiony, agent.width, agent.height);
    significant = EE_MAX(collider.height/2, collider.width/2)+EE_MAX(agent.height/2, agent.width/2);
    if (ccollision::distance(center[EE_RECTANGLE_ONE], center[EE_RECTANGLE_TWO]) <= significant) {
        position[EE_RECTANGLE_ONE] = (int)(fmodf(collider.angle, 360.0f)*10.0f);
        position[EE_RECTANGLE_TWO] = (int)(fmodf(agent.angle, 360.0f)*10.0f);
        sina[EE_RECTANGLE_ONE] = cmath::esina[position[EE_RECTANGLE_ONE]];
        sina[EE_RECTANGLE_TWO] = cmath::esina[position[EE_RECTANGLE_TWO]];
        cosa[EE_RECTANGLE_ONE] = cmath::ecosa[position[EE_RECTANGLE_ONE]];
        cosa[EE_RECTANGLE_TWO] = cmath::ecosa[position[EE_RECTANGLE_TWO]];
        for (axis = 0; axis < EAXIS_NULL; axis++) {
            definition[EE_RECTANGLE_ONE][axis] = ccollision::axis((eaxis)axis, collider.origin.positionx, collider.origin.positiony, collider.width, collider.height, sina[EE_RECTANGLE_ONE], cosa[EE_RECTANGLE_ONE], collider.pivot);
            definition[EE_RECTANGLE_TWO][axis] = ccollision::axis((eaxis)axis, agent.origin.positionx, agent.origin.positiony, agent.width, agent.height, sina[EE_RECTANGLE_TWO], cosa[EE_RECTANGLE_TWO], agent.pivot);
        }
        for (axis = 0; ((axis < EAXIS_NULL) && (!result.collision)); axis++)
            for (subaxis = 0; ((subaxis < EAXIS_NULL) && (!result.collision)); subaxis++)
                result = ccollision::intersect(definition[EE_RECTANGLE_ONE][axis], definition[EE_RECTANGLE_TWO][subaxis]);
    }
    return result;
}

scollision ccollision::collision (ccontext *collider, ccontext *agent) {
    srectangle rects[EE_RECTANGLES];
    rects[EE_RECTANGLE_ONE].origin.positionx = collider->destination(EPOSITION_POSITIONX);
    rects[EE_RECTANGLE_ONE].origin.positiony = collider->destination(EPOSITION_POSITIONY);
    rects[EE_RECTANGLE_ONE].width = collider->destination(EPOSITION_WIDTH);
    rects[EE_RECTANGLE_ONE].height = collider->destination(EPOSITION_HEIGHT);
    rects[EE_RECTANGLE_ONE].angle = collider->rotation;
    rects[EE_RECTANGLE_ONE].pivot.positionx = (collider->destination(EPOSITION_POSITIONX)+(collider->destination(EPOSITION_WIDTH)/2));
    rects[EE_RECTANGLE_ONE].pivot.positiony = (collider->destination(EPOSITION_POSITIONY)+(collider->destination(EPOSITION_HEIGHT)/2));
    rects[EE_RECTANGLE_TWO].origin.positionx = agent->destination(EPOSITION_POSITIONX);
    rects[EE_RECTANGLE_TWO].origin.positiony = agent->destination(EPOSITION_POSITIONY);
    rects[EE_RECTANGLE_TWO].width = agent->destination(EPOSITION_WIDTH);
    rects[EE_RECTANGLE_TWO].height = agent->destination(EPOSITION_HEIGHT);
    rects[EE_RECTANGLE_TWO].angle = agent->rotation;
    rects[EE_RECTANGLE_TWO].pivot.positionx = (agent->destination(EPOSITION_POSITIONX)+(agent->destination(EPOSITION_WIDTH)/2));
    rects[EE_RECTANGLE_TWO].pivot.positiony = (agent->destination(EPOSITION_POSITIONY)+(agent->destination(EPOSITION_HEIGHT)/2));
    return ccollision::collision(rects[EE_RECTANGLE_ONE], rects[EE_RECTANGLE_TWO]);
}

spoint ccollision::rotate (int positionx, int positiony, spoint pivot, float sina, float cosa) {
    spoint value = {positionx, positiony};
    return ccollision::rotate(value, pivot, sina, cosa);
}

spoint ccollision::rotate (spoint point, spoint pivot, float sina, float cosa) {
    spoint result;
    float positionx, positiony, temporary;
    positionx = point.positionx - pivot.positionx;
    positiony = point.positiony - pivot.positiony;
    temporary = positionx;
    positionx = (temporary*cosa)-(positiony*sina);
    positiony = (temporary*sina)+(positiony*cosa);
    result.positionx = positionx + pivot.positionx;
    result.positiony = positiony + pivot.positiony;
    return result;
}

spoint ccollision::pivot (int positionx, int positiony, int width, int height) {
    spoint result;
    result.positionx = (positionx+(width/2));
    result.positiony = (positiony+(height/2));
    return result;
}

spoint ccollision::pivot (ccontext *context) {
    return ccollision::pivot(context->destination(EPOSITION_POSITIONX), context->destination(EPOSITION_POSITIONY), context->destination(EPOSITION_WIDTH), context->destination(EPOSITION_HEIGHT));
}

sline ccollision::axis (eaxis target, int positionx, int positiony, int width, int height, float sina, float cosa, spoint pivot) {
    sline result;
    switch (target) {
        case EAXIS_TOP:
            result.point[0] = ccollision::rotate(positionx, positiony, pivot, sina, cosa);
            result.point[1] = ccollision::rotate((positionx+width), positiony, pivot, sina, cosa);
            break;
        case EAXIS_BOTTOM:
            result.point[0] = ccollision::rotate(positionx, (positiony+height), pivot, sina, cosa);
            result.point[1] = ccollision::rotate((positionx+width), (positiony+height), pivot, sina, cosa);
            break;
        case EAXIS_LEFT:
            result.point[0] = ccollision::rotate(positionx, positiony, pivot, sina, cosa);
            result.point[1] = ccollision::rotate(positionx, (positiony+height), pivot, sina, cosa);
            break;
        case EAXIS_RIGHT:
            result.point[0] = ccollision::rotate((positionx+width), positiony, pivot, sina, cosa);
            result.point[1] = ccollision::rotate((positionx+width), (positiony+height), pivot, sina, cosa);
            break;
        default:
            break;
    }
    return result;
}

scollision ccollision::intersect (sline collider, sline other) {
    scollision result;
    float denominator, s, t;
    spoint BA, DC, AC;
    result.collision = false;
    BA.positionx = coordx(collider,1)-coordx(collider,0);
    BA.positiony = coordy(collider,1)-coordy(collider,0);
    DC.positionx = coordx(other,1)-coordx(other,0);
    DC.positiony = coordy(other,1)-coordy(other,0);
    AC.positionx = coordx(collider,0)-coordx(other,0);
    AC.positiony = coordy(collider,0)-coordy(other,0);
    denominator = (DC.positiony*BA.positionx)-(DC.positionx*BA.positiony);
    s = ((DC.positionx*AC.positiony)-(DC.positiony*AC.positionx));
    t = ((BA.positionx*AC.positiony)-(BA.positiony*AC.positionx));
    if (denominator != 0) {
        s /= denominator;
        if ((s >= 0.0f) && (s <= 1.0f)) {
            t /= denominator;
            if ((t >= 0.0f) && (t <= 1.0f)) {
                result.collision = true;
                result.impact.positionx = (((float)coordx(collider,0)+(s*((float)coordx(collider,1)-(float)coordx(collider,0)))));
                result.impact.positiony = (((float)coordy(collider,0)+(s*((float)coordy(collider,1)-(float)coordy(collider,0)))));
            }
        }
    }
    return result;
}

float ccollision::distance (spoint source, spoint destination) {
    float result = 0;
    result = cmath::sqrt(cmath::pow2(source.positionx-destination.positionx)+cmath::pow2(source.positiony-destination.positiony));
    return result;
}

float ccollision::distance (ccontext *source, ccontext *destination) {
    spoint center[EE_RECTANGLES];
    center[EE_RECTANGLE_ONE] = ccollision::pivot(source->destination(EPOSITION_POSITIONX), source->destination(EPOSITION_POSITIONY), source->destination(EPOSITION_WIDTH), source->destination(EPOSITION_HEIGHT));
    center[EE_RECTANGLE_TWO] = ccollision::pivot(destination->destination(EPOSITION_POSITIONX), destination->destination(EPOSITION_POSITIONY), destination->destination(EPOSITION_WIDTH), destination->destination(EPOSITION_HEIGHT));
    return ccollision::distance(center[EE_RECTANGLE_ONE], center[EE_RECTANGLE_TWO]);
}

sdirection ccollision::position (spoint origin, spoint target) {
    sdirection result;
    int direction = EDIRECTION_NULL;
    result.direction = EDIRECTION_NULL;
    result.horizontal = result.vertical = 0;
    if (origin.positionx > target.positionx) {
        direction += EDIRECTION_LEFT;
        result.horizontal = origin.positionx - target.positionx;
    } else if (origin.positionx < target.positionx) {
        direction += EDIRECTION_RIGHT;
        result.horizontal = target.positionx - origin.positionx;
    }
    if (origin.positiony > target.positiony) {
        direction += EDIRECTION_TOP;
        result.vertical = origin.positiony - target.positiony;
    } else if (origin.positiony < target.positiony) {
        direction += EDIRECTION_BOTTOM;
        result.vertical = target.positiony - origin.positiony;
    }
    result.direction = (edirection)direction;
    return result;
}
