/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Filippo Paolucci
 * EMAIL        : fantic86@alice.it
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "cfx.h"
void cfx::init(void) {
    cmodel::init();
    offsetx = offsety = EF_FX_OFFSET;
    basex = basey = EF_FX_BASE;
    distance = EF_FX_DISTANCE;
    speed = EF_FX_SPEED;
    angle = EF_FX_ANGLE;
    fxlayer = EF_FX_LAYER;
    lifetime_msecs = 0;
    initialized = false;
    fxposition = EFXPOSITION_NOT_FOLLOW;
}

int cfx::initialize (sdfssegment* datablock, int layer) {
    int result = EE_OK;
    if (layer >= 0) 
        this->fxlayer = layer;
    if ((result = cmodel::initialize(datablock))) {
        initialized = true;
        if (EE_CFX_PRINT) 
            elog("datablock: cfx initialization completed");
    } else
        elog("datablock: cfx initialization failed");
    return result;
}

int cfx::initialize (cmodel* model, bool hard, int layer) {
    int result = EE_OK;
    if (layer >= 0)
        this->fxlayer = layer;
    if ((result = cmodel::initialize(model, hard))) {
        initialized = true;
        if (EE_CFX_PRINT)
            elog("datablock: cfx initialization completed");
    } else
        elog("datablock: cfx initialization failed");
    return result;
}

int cfx::copyfx (cfx* fx, bool hard) {
    int result = EE_OK;
    if (initialize((cmodel*)fx, hard)) {
        basex = fx->basex;
        basey = fx->basey;
        fxtype = fx->fxtype;
        fxposition = fx->fxposition;
        distance = fx->distance;
        speed = fx->speed;
        angle = fx->angle;
        offsetx = fx->offsetx;
        offsety = fx->offsety;
        lifetime_msecs = fx->lifetime_msecs;
        initialized = fx->initialized;
        if (EE_CFX_PRINT) 
            elog("fx: effect copy completed");
    } else {
        result = EE_ERROR;
        elog("fx: impossible to copy fx model in clone mode");
    }
    return result;
}

int cfx::setfx (sfxcircle circle_setting, efxposition fxposition, int lifetime_msecs, int offsetx, int offsety) {
    int result = EE_OK;
    fxtype = EFXTYPE_CIRCLE;
    distance = circle_setting.radius;
    speed = circle_setting.rotation_speed;
    angle = circle_setting.start_angle;
    setfx(fxposition, lifetime_msecs, offsetx, offsety);
    return result;
}

int cfx::setfx(sfxrect rect_setting, efxposition fxposition, int lifetime_msecs, int offsetx, int offsety) {
    int result = EE_OK;
    fxtype = EFXTYPE_RECT;
    distance = rect_setting.start_distance;
    speed = rect_setting.speed;
    angle = rect_setting.direction_angle;
    setfx(fxposition, lifetime_msecs, offsetx, offsety);
    return result;
}

int cfx::setfx(efxposition fxposition, int lifetime_msecs, int offsetx, int offsety) {
    int result = EE_OK;
    this->fxposition = fxposition;
    this->offsetx = offsetx;
    this->offsety = offsety;
    this->lifetime_msecs = lifetime_msecs;
    if (EE_CFX_PRINT) 
        elog("fx: effect setting up correctly");
    return result;
}
