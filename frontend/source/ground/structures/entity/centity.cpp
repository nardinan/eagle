/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Filippo Paolucci
 * EMAIL        : fantic86@alice.it
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "centity.h"
#include "ceffects.h"
void centity::init (void) {
    modeltypessize = EE_BUCKET_SIZE;
    modeltypescount = 0;
    fxcount = 0;
    instancetime = NULL;
    strapp(instancetime, "classstartime");
    centerx = centery = EF_ENTITY_CENTER;
    if ((fxmodels = enew carray<cfx > (modeltypessize)))
        if (timer = enew ctimer())
            if ((fxlist = enew carray<cfx > (modeltypessize)))
                if (model = enew cmodel())
                    timer->add(instancetime);
    elog("entity: initialization failed");
}

int centity::initialize (cmodel* model, const char* label, int layer) {
    int result = EE_OK;
    if (estrlen(label) > 0) {
        if (model) {
            this->model->initialize(model, true);
            cengine::environment[EENVIRONMENT_OVER].add(this->model, layer, label);
        } else {
            result = EE_ERROR;
            elog("model: object not initialized");
        }
    } else {
        result = EE_ERROR;
        elog("label: variable label not initialized");
    }
    return result;
}

int centity::set(int x, int y) {
    int result = EE_OK;
    if (model) {
        this->model->drop(EPOSITION_POSITIONX, x);
        this->model->drop(EPOSITION_POSITIONY, y);
        centerx = (int) x + this->model->destination(EPOSITION_WIDTH) / 2.0f;
        centery = (int) y + this->model->destination(EPOSITION_HEIGHT) / 2.0f;
    } else {
        result = EE_ERROR;
        elog("model: object not initialized");
    }
    return result;
}

int centity::setposx (int x) {
    int result = EE_OK;
    if (model) {
        this->model->drop(EPOSITION_POSITIONX, x);
        centerx = (int) x + this->model->destination(EPOSITION_WIDTH) / 2.0f;
    } else {
        result = EE_ERROR;
        elog("model: object not initialized");
    }
    return result;
}

int centity::setposy (int y) {
    int result = EE_OK;
    if (model) {
        this->model->drop(EPOSITION_POSITIONY, y);
        centery = (int) y + this->model->destination(EPOSITION_HEIGHT) / 2.0f;
    } else {
        result = EE_ERROR;
        elog("model: object not initialized");
    }
    return result;
}

int centity::setcollision (bool activate_collision, fimpacthandler* collision_handler) {
    spoint p = {0, 0};
    int result = EE_OK;
    if (collision_handler != NULL) {
        this->collision_handler = collision_handler;
        if (this->model) {
            memset(this->nomenclature, '\0', EF_NOMENCLATURE_SIZE);
            strand(this->nomenclature, (EF_NOMENCLATURE_SIZE-1));
            cengine::impacts.add(model, this->nomenclature, collision_handler);
            cengine::impacts.box(this->nomenclature, p, getw(model), geth(model));
        } else {
            result = EE_ERROR;
            elog("model: object not initialized");
        }
    } else {
        result = EE_ERROR;
        elog("model: collision handler not initialized");
    }
    return result;
}

int centity::addfx (const char* ceffects_label) {
    cfx* fx = NULL;
    int result = EE_OK;
    if (estrlen(ceffects_label) > 0) {
        if ((fx = ceffects::getfx(ceffects_label))) {
            if (EE_CENTITY_PRINT) elog("label: %s model added to entity correctly", ceffects_label);
            result = addfx(fx, ceffects_label);
        } else {
            result = EE_ERROR;
            elog("label: no effect labeled %s finded in effect list");
        }
    } else {
        result = EE_ERROR;
        elog("label: variable not inizialized");
    }
    return result;
}

int centity::addfx (cfx* fx, const char* label) {
    cfx* newfx = NULL;
    int result = EE_OK;
    if (fx) {
        if (estrlen(label) > 0) {
            if ((newfx = enew cfx())) {
                if ((result = newfx->copyfx(fx, true))) {
                    modeltypescount++;
                    if ((result = fxmodels->add(newfx, label))) {
                        if (EE_CENTITY_PRINT)
                            elog("fx: %s added to modeltypes correctly", label);
                    } else {
                        result = EE_ERROR;
                        elog("fx: impossible add to modeltypes");
                    }
                } else {
                    result = EE_ERROR;
                    elog("fx: impossible to copy modeltypes");
                }
                if (!result) {
                    newfx->unload();
                    delete newfx;
                }
            } else
                ekill("out of memory");
        } else {
            result = EE_ERROR;
            elog("label: variable not inizialized");
        }
    } else {
        result = EE_ERROR;
        elog("fx: variable not inizialized");
    }
    return result;
}

int centity::createfx (const char* label) {
    sagent *master = NULL;
    cfx *fxmodel = NULL, *istancefx, *fx;
    spoint p = {0, 0};
    char instancelabel[EE_LABEL_SIZE];
    int result = EE_OK;
    if (estrlen(label) > 0) {
        strand(instancelabel, 8);
        if ((fxmodel = fxmodels->get(label)) != NULL) {
            if (EE_CENTITY_PRINT)
                elog("label: model %s finded in modeltypes", label);
            if ((istancefx = enew cfx())) {
                if ((result = istancefx->copyfx(fxmodel, true))) {
                    istancefx->basex = istancefx->offsetx + centerx - (int) istancefx->destination(EPOSITION_WIDTH) / 2.0f;
                    istancefx->basey = istancefx->offsety + centery - (int) istancefx->destination(EPOSITION_HEIGHT) / 2.0f;
                    fxcount++;
                    if ((result = fxlist->add(istancefx, instancelabel))) {
                        timer->add(instancelabel);
                        forwardfx();
                        if (EE_CENTITY_PRINT)
                            elog("label: %s instance fx generated", instancelabel);
                        killfx(istancefx->lifetime_msecs);
                        fx = fxlist->get(instancelabel);
                        cengine::environment[EENVIRONMENT_OVER].add(fx, fx->fxlayer, instancelabel);
                        if (this->collision_handler != NULL) {
                            cengine::impacts.add(fx, instancelabel, collision_handler);
                            cengine::impacts.box(instancelabel, p, getw(fx), geth(fx));
                            master = cengine::impacts.get(instancelabel);
                            master->group = 10;
                        }
                    } else {
                        result = EE_ERROR;
                        elog("fx: can't add %s, duplicate of %s, to fx list", instancelabel, label);
                    }
                } else {
                    result = EE_ERROR;
                    elog("fx: can't duplicate fx %s", label);
                }
                if (!result) {
                    istancefx->unload();
                    delete istancefx;
                }
            } else
                ekill("out of memory");
        } else {
            result = EE_ERROR;
            elog("label: model %s not finded in modeltypes", label);
        }
    } else {
        result = EE_ERROR;
        elog("label: variable not inizialized");
    }
    return result;
}

int centity::forwardfx (void) {
    stime time;
    cfx* fx;
    char* fxlabel;
    float usecs, angle;
    int valx, valy, result = EE_OK;
    if (model) {
        foreach(fx, fxlist) {
            fxlabel = this->fxlist->label();
            if (fx != NULL) {
                valx = fx->basex;
                valy = fx->basey;
                angle = fmod(fx->angle / 180.0f * 3.14, 6.28);
                if (timer) {
                    time = timer->get(fxlabel);
                    if (fx->fxposition == EFXPOSITION_FOLLOW) {
                        fx->basex = fx->offsetx + centerx - (int) fx->destination(EPOSITION_WIDTH) / 2.0f;
                        fx->basey = fx->offsety + centery - (int) fx->destination(EPOSITION_HEIGHT) / 2.0f;
                    }
                    if (fx->fxtype == EFXTYPE_RECT) {
                        usecs = time.usecs;
                        valx += (int) (cos(angle) * (fx->distance + fx->speed * usecs / 5000.0f));
                        valy -= (int) (sin(angle) * (fx->distance + fx->speed * usecs / 5000.0f));
                        fx->rotation = angle;
                    }
                    if (fx->fxtype == EFXTYPE_CIRCLE) {
                        angle = fmod(angle + time.usecs / (1000000.0f / fx->speed)*6.28, 6.28);
                        valy += (int) (sin(angle) * fx->distance * fx->speed);
                        valx -= (int) (cos(angle) * fx->distance * fx->speed);
                        fx->rotation += 0.0f;
                    }
                }
                fx->drop(EPOSITION_POSITIONX, valx);
                fx->drop(EPOSITION_POSITIONY, valy);
            }
        }
    } else
        result = EE_ERROR;
    return result;
}

int centity::killfx (int msecs) {
    char* fxlabel;
    stime time;
    cfx* fx;
    int result = EE_OK;
    if (msecs > 0) {
        if (EE_CENTITY_PRINT)
            elog("fxlist: number of effects in entity %d real %d", fxcount, fxlist->records());
        foreach(fx, fxlist) {
            fxlabel = this->fxlist->label();
            if (fxlabel) {
                time = timer->get(fxlabel);
                if (msecs != 0 && time.usecs > msecs * 1000) {
                    if (cengine::environment[EENVIRONMENT_OVER].get((ccontext *) fx)) {
                        cengine::environment[EENVIRONMENT_OVER].del();
                        this->fxlist->del();
                        if (EE_CENTITY_PRINT)
                            elog("fx: %s fx killed after %u lifetime", fxlabel, (int) (time.usecs / 1000.0f));
                        fx->unload();
                        delete fx;
                        fxcount--;
                    }
                }
            }
        }
    } else
        result = EE_ERROR;
    return result;
}
