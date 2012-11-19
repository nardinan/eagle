/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CIMPACTS_H
#define EE_CIMPACTS_H
#include "crandom.h"
#include "carray.h"
#include "ccollision.h"
#define EE_IMPACT_NOGROUP -1
typedef struct sarea {
    spoint offset;
    unsigned int width, height;
} sarea;
typedef struct sagent {
    void *informations;
    unsigned int group;
    ccontext *context;
} sagent;
typedef void (fimpacthandler)(sagent *collider, sagent *agent, spoint impact);
typedef struct scollider {
    sagent *agent;
    sarea cover;
    carray<sarea> boxes;
    fimpacthandler *handler;
} scollider;
class cimpacts:public eagleclass {
friend class cengine;
private:
    carray<scollider> colliders;
public:
    inline cimpacts (void) { this->init(); };
    void init (void);
    int add (ccontext *context, const char *label, fimpacthandler *handler);
    int box (const char *label, spoint offset, unsigned int width, unsigned int height);
    int cover (const char *label, spoint offset, unsigned int width, unsigned int height);
    sagent *get (const char *label);
    void del (const char *label);
protected:
    void recalculate (void);
    void unload (scollider *singleton);
public:
    void unload (void);
};
#endif