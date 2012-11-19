/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CPATHS_H
#define EE_CPATHS_H
#include "carray.h"
#include "ccontext.h"
#include "ctimer.h"
#include "ccollision.h"
#define EE_PATHS_MOVEMENT 1
typedef struct spath {
    spoint destination;
    unsigned int running, update[EPOSITION_NULL];
    float rotation;
    bool destroy, executing;
} spath;
typedef struct smovement {
    ccontext *context;
    carray<spath> paths;
    ctimer stopwatch;
} smovement;
class cpaths:public eagleclass {
friend class cengine;
private:
    carray<smovement> movements;
public:
    inline cpaths (void) { this->init(); };
    void init (void);
    int add (ccontext *context, const char *label);
    int add (unsigned int positionx, unsigned int positiony, unsigned int running, const char *label, bool destroy=true);
    inline smovement *get (const char *label) { return movements.get(label); };
    void del (const char *label);
    int fflush (const char *label);
protected:
    void recalculate (void);
    void unload (smovement *singleton);
public:
    void unload (void);
};
#endif