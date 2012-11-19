/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CMODEL_H
#define EE_CMODEL_H
#include "cxml.h"
#include "ctarga.h"
#include "ctimer.h"
#define EE_MODEL_UPDATE 11000
#define EE_MODEL_INFINITE_LOOP -1
typedef struct sframe {
    char context[EE_LABEL_SIZE];
    int overlay[EPOSITION_NULL];
    float angle;
    time_t update;
} sframe;
typedef struct saction {
    char label[EE_LABEL_SIZE];
    int loop, current;
    ctimer stopwatch;
    carray<sframe> frames;
} saction;
class cmodel:public ccontext {
protected:
    carray<ctarga> elements;
    carray<saction> actions;
    bool returned, hard;
public:
    inline cmodel (void) { this->init(); };
    void init (void);
    int initialize (sdfssegment *datablock);
    int initialize (cmodel *model, bool hard=true);
protected:
    int action (sxmlclass *xmlclass);
    int frame (sxmlclass *xmlclass, saction *action);
public:
    int set (const char *label);
    int invisibility (scolor color, unsigned int threshold=0, bool revealing=true);
    /* parent's virtual redefiniton */
    scolor *pixel (unsigned int positionx, unsigned int positiony);
    GLuint get (void);
    void restore (void);
private:
    void unload (saction *action);
    void unload (ctarga *targa);
public:
    void unload (void);
};
#endif
