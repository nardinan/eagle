/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Filippo Paolucci
 * EMAIL        : fantic86@alice.it
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EF_ENTITY
#define EF_ENTITY
#include "cengine.h"
#include "cfx.h"
#include "debug.h"
#ifdef EE_CENTITY_PRINT
#undef EE_CENTITY_PRINT
#define EE_CENTITY_PRINT 1
#else 
#define EE_CENTITY_PRINT 0
#endif
#define EE_NOMENCLATURE_SIZE 8
class centity: public eagleclass {
private:
    cmodel* model;
    int centerx;
    int centery;
    int modeltypessize;
    int modeltypescount;
    int fxcount;
    int spawnx, spawny;
    ctimer* timer;
    char* instancetime;
    char nomenclature[EE_NOMENCLATURE_SIZE];
public:
    fimpacthandler* collision_handler;
    carray<cfx> *fxlist;
    carray<cfx> *fxmodels;
    inline centity() { this->init();}
    int init ();
    int initialize (cmodel* model, const char* label="model",int layer=1);
    int set (int x , int y);
    int setposx (int x);
    int setposy (int y);
    inline int getposx (){return this->model->destination(EPOSITION_POSITIONX);}
    inline int getposy (){return this->model->destination(EPOSITION_POSITIONY);}
    inline int getwidth () { return this->model->destination(EPOSITION_WIDTH) ;}
    inline int getheight () { return this->model->destination(EPOSITION_HEIGHT) ;}
    inline int getcenterx () { return this->centerx; }
    inline int getcentery () { return this->centery; }
    int addfx (const char* ceffects_label);
    int addfx (cfx* model, const char* label);
    int setcollision (bool activate_collision, fimpacthandler* collision_handler);
    int delfx ();
    int createfx (const char* label);
    int forwardfx ();
    int killfx (int msecs);
    inline void unload () { 
        destroy(fxlist,unload,cfx);
        delete fxlist;
        destroy(fxmodels,unload,cfx);
        delete fxmodels;
        //TODO: ATTENZIONE SE TOLGO IL MODO DI COPIARE DA TRUE A FALSE DI MODEL NON DEVO PIU CANCELLARLI
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
