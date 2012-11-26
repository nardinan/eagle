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
    if (!(ceffects::cfxlist = enew carray<cfx>()))
        ekill("out of memory");
}

int ceffects::addfx (cfx* fx, const char* label) {
    cfx* fxmodel = NULL;
    int result = EE_OK;
    if (fx) {
        if ((fxmodel = enew cfx())) {
            if ((result = fxmodel->copyfx(fx, true))) {
                if (estrlen(label) > 0) {
                    if ((result = ceffects::cfxlist->add(fxmodel, label))) {
                        if (EE_CEFFECTS_PRINT)
                            elog("fx: %s added to ceffects correctly", label);
                    } else
                        elog("fx: impossible add to ceffects");
                } else {
                    result = EE_ERROR;
                    elog("label: variable not inizialized");
                }
            }
        } else
            ekill("out of memory");
    } else {
        result = EE_ERROR;
        elog("fx: variable not inizialized");
    }
    return EE_ERROR;
}

int ceffects::delfx(const char* label) {
    cfx* fx = NULL;
    char *fxlabel = NULL;
    int result = EE_OK;
    bool founded = true;
    foreach(fx, ceffects::cfxlist) {
        fxlabel = ceffects::cfxlist->label();
        if (estrcmp(fxlabel, label) == 0) {
            ceffects::cfxlist->del();
            if (EE_CEFFECTS_PRINT)
                elog("label: effect deleted from list correctly");
            fx->unload();
            delete fx;
            founded = true;
            break;
        }
    }
    if (!founded) {
        result = EE_ERROR;
        elog("label: %s not exist in fx list", label);
    }
    return result;
}

int ceffects::setfx(const char* label, sfxcircle circle_setting, efxposition fxposition, int lifetime_msecs, int offsetx, int offsety) {
    cfx* fx = NULL;
    char *fxlabel = NULL;
    int result = EE_OK;
    bool founded = true;
    foreach(fx, ceffects::cfxlist) {
        fxlabel = ceffects::cfxlist->label();
        if (estrcmp(fxlabel, label) == 0) {
            if ((result = fx->setfx(circle_setting, fxposition, lifetime_msecs, offsetx, offsety))) {
                if (EE_CEFFECTS_PRINT)
                    elog("label: %s effect setup completed", label);
            } else {
                result = EE_ERROR;
                elog("label: can't set effect %s", label);
            }
            founded = true;
            break;
        }
    }
    if (!founded) {
        result = EE_ERROR;
        elog("label: %s not exist in fx list", label);
    }
    return result;
}

int ceffects::setfx(const char* label, sfxrect rect_setting, efxposition fxposition, int lifetime_msecs, int offsetx, int offsety) {
    cfx* fx = NULL;
    char *fxlabel = NULL;
    int result = EE_OK;
    bool founded = true;
    foreach(fx, ceffects::cfxlist) {
        fxlabel = ceffects::cfxlist->label();
        if (estrcmp(fxlabel, label) == 0) {
            if ((result = fx->setfx(rect_setting, fxposition, lifetime_msecs, offsetx, offsety))) {
                if (EE_CEFFECTS_PRINT)
                    elog("label: %s effect setup completed", label);
            } else {
                result = EE_ERROR;
                elog("label: can't set effect %s", label);
            }
            founded = true;
            break;
        }
    }
    if (!founded) {
        result = EE_ERROR;
        elog("label: %s not exist in fx list", label);
    }
    return EE_ERROR;
}
