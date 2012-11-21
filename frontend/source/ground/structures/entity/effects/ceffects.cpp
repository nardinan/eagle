/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Filippo Paolucci
 * EMAIL        : fantic86@alice.it
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "ceffects.h"
carray<cfx> *ceffects::cfxlist;
void ceffects::init (void) {
    ceffects::cfxlist = enew carray<cfx > ();
}

int ceffects::addfx (cfx* fx, const char* label) {
    if (fx) {
        cfx* fxmodel = enew cfx();
        fxmodel->copyfx(fx, true);
        if (label && strcmp(label, "") != 0) {
            if (ceffects::cfxlist->add(fxmodel, label)) {
                if (EE_CEFFECTS_PRINT) elog("fx: %s added to ceffects correctly", label);
                return EE_OK;
            } else
                elog("fx: impossible add to ceffects");
        } else
            elog("label: variable not inizialized");
    } else
        elog("fx: variable not inizialized");
    return EE_ERROR;
}

cfx* ceffects::getfx(const char* label) {
    return cfxlist->get(label);
}

int ceffects::delfx(const char* label) {
    cfx* fx = NULL;
    foreach(fx, ceffects::cfxlist) {
        char* fxlabel = ceffects::cfxlist->label();
        if (strcmp(fxlabel, label) == 0) {
            ceffects::cfxlist->del();
            if (EE_CEFFECTS_PRINT) elog("label: effect deleted from list correctly");
            fx->unload();
            delete fx;
            return EE_OK;
        }
    }
    elog("label: %s not exist in fx list", label);
    return EE_ERROR;
}

int ceffects::setfx(const char* label, sfxcircle circle_setting, efxposition fxposition, int lifetime_msecs, int offsetx, int offsety) {
    cfx* fx = NULL;
    foreach(fx, ceffects::cfxlist) {
        char* fxlabel = ceffects::cfxlist->label();
        if (strcmp(fxlabel, label) == 0) {
            fx->setfx(circle_setting, fxposition, lifetime_msecs, offsetx, offsety);
            if (EE_CEFFECTS_PRINT) elog("label: %s effect setup completed", label);
            return EE_OK;
        }
    }
    elog("label: %s not exist in fx list", label);
    return EE_ERROR;
}

int ceffects::setfx(const char* label, sfxrect rect_setting, efxposition fxposition, int lifetime_msecs, int offsetx, int offsety) {
    cfx* fx = NULL;
    foreach(fx, ceffects::cfxlist) {
        char* fxlabel = ceffects::cfxlist->label();
        if (strcmp(fxlabel, label) == 0) {
            fx->setfx(rect_setting, fxposition, lifetime_msecs, offsetx, offsety);
            if (EE_CEFFECTS_PRINT) elog("label: %s effect setup completed", label);
            return EE_OK;
        }
    }
    elog("label: %s not exist in fx list", label);
    return EE_ERROR;
}

sfxcircle ceffects::getcircle_setting(float start_angle, float rotation_speed, int radius) {
    sfxcircle circle = {start_angle, rotation_speed, radius};
    return circle;
}

sfxrect ceffects::getrect_setting(float direction_angle, float speed, int start_distance) {
    sfxrect rect = {direction_angle, speed, start_distance};
    return rect;
}
