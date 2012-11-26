/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Filippo Paolucci
 * EMAIL        : fantic86@alice.it
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EF_CENTITY
#define EF_CENTITY
#ifndef _WIN32
#include <engine/cengine.h>
#else
#include <cengine.h>
#endif
#include "cfx.h"
#include "debug.h"
#define EF_NOMENCLATURE_SIZE 8
#define EF_ENTITY_CENTER 0
class centity: public eagleclass {
private:
    cmodel* model;
    int centerx, centery, modeltypessize, modeltypescount, fxcount, spawnx, spawny;
    ctimer* timer;
    char *instancetime, nomenclature[EF_NOMENCLATURE_SIZE];
public:
    fimpacthandler *collision_handler;
    carray<cfx> *fxlist;
    carray<cfx> *fxmodels;
    inline centity(void) { this->init();}
    void init (void);
    int initialize (cmodel* model, const char* label="model",int layer=1);
    int set (int x, int y);
    int setposx (int x);
    int setposy (int y);
    inline int getposx (void) { return this->model->destination(EPOSITION_POSITIONX); };
    inline int getposy (void) { return this->model->destination(EPOSITION_POSITIONY); };
    inline int getwidth (void) { return this->model->destination(EPOSITION_WIDTH); };
    inline int getheight (void) { return this->model->destination(EPOSITION_HEIGHT); };
    inline int getcenterx (void) { return this->centerx; };
    inline int getcentery (void) { return this->centery; };
    int addfx (const char* ceffects_label);
    int addfx (cfx* model, const char* label);
    int setcollision (bool activate_collision, fimpacthandler* collision_handler);
    int delfx (void);
    int createfx (const char* label);
    int forwardfx (void);
    int killfx (int msecs);
    inline void unload (void) { 
        destroy(fxlist,unload,cfx);
        delete fxlist;
        destroy(fxmodels,unload,cfx);
        delete fxmodels;
        /*
            TODO: attenzione, se tolgo il modo di copiare da true a false di model non devo piu cancellarli
         */
        model->unload();
        delete model;
        timer->unload();
        delete timer;
        efree(instancetime);
    } 
private:
    inline void unload (cfx* fx) {
        fx->unload();
        delete fx;
    }
};
#endif
