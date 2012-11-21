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
void cfx::init() {
    cmodel::init();
    offsetx = offsety = 0;
    distance = 10;
    speed = 1.0f;
    angle = 90.0f;
    basex = 0;
    basey = 0;
    lifetime_msecs = 0;
    fxlayer = 1;
    initialized = false;
    fxposition = EFXPOSITION_NOT_FOLLOW;
}

int cfx::initialize(sdfssegment* datablock, int layer) {
    if (layer >= 0) 
        this->fxlayer = layer;
    int result = cmodel::initialize(datablock);
    if (result) {
        initialized = true;
        if (EE_CFX_PRINT) 
            elog("datablock: cfx initialization completed");
    } else
        elog("datablock: cfx initialization failed");
    return result;
}

int cfx::initialize(cmodel* model, bool hard, int layer) {
    if (layer >= 0) this->fxlayer = layer;
    int result = cmodel::initialize(model, hard);
    if (result) {
        initialized = true;
        if (EE_CFX_PRINT)
            elog("datablock: cfx initialization completed");
    } else
        elog("datablock: cfx initialization failed");
    return result;
}

int cfx::copyfx(cfx* fx, bool hard) {
    if (initialize((cmodel*) fx, hard)) {
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
        return EE_OK;
    }
    elog("fx: impossible to copy fx model in clone mode");
    return EE_ERROR;
}

int cfx::setfx(sfxcircle circle_setting, efxposition fxposition, int lifetime_msecs, int offsetx, int offsety) {
    fxtype = EFXTYPE_CIRCLE;
    distance = circle_setting.radius;
    speed = circle_setting.rotation_speed;
    angle = circle_setting.start_angle;
    setfx(fxposition, lifetime_msecs, offsetx, offsety);
    return EE_OK;
}

int cfx::setfx(sfxrect rect_setting, efxposition fxposition, int lifetime_msecs, int offsetx, int offsety) {
    fxtype = EFXTYPE_RECT;
    distance = rect_setting.start_distance;
    speed = rect_setting.speed;
    angle = rect_setting.direction_angle;
    setfx(fxposition, lifetime_msecs, offsetx, offsety);
    return EE_OK;
}

int cfx::setfx(efxposition fxposition, int lifetime_msecs, int offsetx, int offsety) {
    this->fxposition = fxposition;
    this->offsetx = offsetx;
    this->offsety = offsety;
    this->lifetime_msecs = lifetime_msecs;
    if (EE_CFX_PRINT) 
        elog("fx: effect setting up suceed");
    return EE_OK;
}
