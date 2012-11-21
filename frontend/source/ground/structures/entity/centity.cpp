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
int centity::init () {
    modeltypessize = EE_BUCKET_SIZE;
    modeltypescount = 0;
    fxcount = 0;
    instancetime = NULL;
    strapp(instancetime, "classstartime");
    centerx = 0;
    centery = 0;
    if ((fxmodels = enew carray<cfx > (modeltypessize)) != NULL)
        if (timer = enew ctimer())
            if ((fxlist = enew carray<cfx > (modeltypessize)) != NULL)
                if (model = enew cmodel()) {
                    timer->add(instancetime);
                    return EE_OK;
                }
    elog("entity: initialization failed");
    return EE_ERROR;
}

int centity::initialize (cmodel* model, const char* label, int layer) {
    if (label && strcmp(label, "") != 0) {
        if (model) {
            this->model->initialize(model, true);
            cengine::environment[EENVIRONMENT_OVER].add(this->model, layer, label);
        } else
            elog("model: object not initialized");
    } else
        elog("label: variable label not initialized");
}

int centity::set(int x, int y) {
    if (model) {
        this->model->drop(EPOSITION_POSITIONX, x);
        this->model->drop(EPOSITION_POSITIONY, y);
        centerx = (int) x + this->model->destination(EPOSITION_WIDTH) / 2.0f;
        centery = (int) y + this->model->destination(EPOSITION_HEIGHT) / 2.0f;
        return EE_OK;
    } else
        elog("model: object not initialized");
    return EE_ERROR;
}

int centity::setposx (int x) {
    if (model) {
        this->model->drop(EPOSITION_POSITIONX, x);
        centerx = (int) x + this->model->destination(EPOSITION_WIDTH) / 2.0f;
        return EE_OK;
    } else
        elog("model: object not initialized");
    return EE_ERROR;
}

int centity::setposy (int y) {
    if (model) {
        this->model->drop(EPOSITION_POSITIONY, y);
        centery = (int) y + this->model->destination(EPOSITION_HEIGHT) / 2.0f;
        return EE_OK;
    } else
        elog("model: object not initialized");
    return EE_ERROR;
}

int centity::setcollision (bool activate_collision, fimpacthandler* collision_handler) {
    if (collision_handler != NULL) {
        this->collision_handler = collision_handler;
        if (this->model) {
            memset(this->nomenclature, '\0', EE_NOMENCLATURE_SIZE);
            strand(this->nomenclature, (EE_NOMENCLATURE_SIZE-1));
            cengine::impacts.add(model, this->nomenclature, collision_handler);
            spoint p = {0, 0};
            cengine::impacts.box(this->nomenclature, p, getw(model), geth(model));
            return EE_OK;
        }
    }
}

int centity::addfx (const char* ceffects_label) {
    if (ceffects_label && strcmp(ceffects_label, "") != 0) {
        cfx* fx = NULL;
        if ((fx = ceffects::getfx(ceffects_label))) {
            if (EE_CENTITY_PRINT) elog("label: %s model added to entity correctly", ceffects_label);
            addfx(fx, ceffects_label);
            return EE_OK;
        } else
            elog("label: no effect labeled %s finded in effect list");
    } else
        elog("label: variable not inizialized");
    return EE_ERROR;
}

int centity::addfx (cfx* fx, const char* label) {
    if (fx) {
        cfx* newfx = enew cfx();
        newfx->copyfx(fx, true);
        if (label && strcmp(label, "") != 0) {
            modeltypescount++;
            if (fxmodels->add(newfx, label)) {
                if (EE_CENTITY_PRINT)
                    elog("fx: %s added to modeltypes correctly", label);
                return EE_OK;
            } else
                elog("fx: impossible add to modeltypes");
        } else
            elog("label: variable not inizialized");
        newfx->unload();
        delete newfx;
    } else
        elog("fx: variable not inizialized");
    return EE_ERROR;
}

int centity::createfx (const char* label) {
    if (label && strcmp(label, "") != 0) {
        char instancelabel[EE_LABEL_SIZE];
        int time = timer->get(instancetime).secs * 1000000 + timer->get(instancetime).usecs + rand() % 100;
        snprintf(instancelabel, EE_LABEL_SIZE, "%u%s", time, label);
        cfx* fxmodel = NULL;
        if ((fxmodel = fxmodels->get(label)) != NULL) {
            cfx* istancefx;
            if (EE_CENTITY_PRINT)
                elog("label: model %s finded in modeltypes", label);
            if ((istancefx = enew cfx())) {
                istancefx->copyfx(fxmodel, true);
                istancefx->basex = istancefx->offsetx + centerx - (int) istancefx->destination(EPOSITION_WIDTH) / 2.0f;
                istancefx->basey = istancefx->offsety + centery - (int) istancefx->destination(EPOSITION_HEIGHT) / 2.0f;
                fxcount++;
                if (fxlist->add(istancefx, instancelabel)) {
                    timer->add(instancelabel);
                    forwardfx();
                    if (EE_CENTITY_PRINT)
                        elog("label: %s instance fx generated", instancelabel);
                }
                killfx(istancefx->lifetime_msecs);
                cfx* fx = fxlist->get(instancelabel);
                cengine::environment[EENVIRONMENT_OVER].add(fx, fx->fxlayer, instancelabel);
                if (this->collision_handler != NULL) {
                    cengine::impacts.add(fx, instancelabel, collision_handler);
                    spoint p = {0, 0};
                    cengine::impacts.box(instancelabel, p, getw(fx), geth(fx));
                    sagent *master = cengine::impacts.get(instancelabel);
                    master->group = 10;
                }
                fflush(stdout);
                return EE_OK;
            } else {
                elog("istancefx: impossible to create a new cfx istance", label);
                istancefx->unload();
                delete istancefx;
            }
        } else
            elog("label: model %s not finded in modeltypes", label);
    } else
        elog("label: variable not inizialized");
    return EE_ERROR;
}

int centity::forwardfx () {
    if (model) {
        cfx* fx;
        foreach(fx, fxlist) {
            char* fxlabel = this->fxlist->label();
            if (fx != NULL) {
                int valx = fx->basex;
                int valy = fx->basey;
                float angle = fmod(fx->angle / 180.0f * 3.14, 6.28);
                if (timer) {
                    stime time = timer->get(fxlabel);
                    if (fx->fxposition == EFXPOSITION_FOLLOW) {
                        fx->basex = fx->offsetx + centerx - (int) fx->destination(EPOSITION_WIDTH) / 2.0f;
                        fx->basey = fx->offsety + centery - (int) fx->destination(EPOSITION_HEIGHT) / 2.0f;
                    }
                    if (fx->fxtype == EFXTYPE_RECT) {
                        float usecs = time.usecs;
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
        return EE_OK;
    }
    return EE_ERROR;
}

int centity::killfx (int msecs) {
    if (msecs > 0) {
        cfx* fx;
        fxlist->reset();
        if (EE_CENTITY_PRINT)
            elog("fxlist: number of effects in entity %d real %d", fxcount, fxlist->records());
        foreach(fx, fxlist) {
            char* fxlabel = this->fxlist->label();
            if (fxlabel) {
                stime time = timer->get(fxlabel);
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
    }
    return EE_ERROR;
}
