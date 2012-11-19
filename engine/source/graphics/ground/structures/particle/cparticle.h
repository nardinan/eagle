/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CPARTICLE_H
#define EE_CPARTICLE_H
#include "ctarga.h"
#include "ctimer.h"
#include "cenvironment.h"
#include "ccollision.h"
#define EE_PARTICLE_FROM 0
#define EE_PARTICLE_TO 1
#define EE_PARTICLE_RANGE 2
#define EE_PARTICLE_TTU 20000
#define EE_PARTICLE_TTL 200000
#define EE_PARTICLE_OVERFLOW_X 0
#define EE_PARTICLE_OVERFLOW_Y 0
#define EE_PARTICLE_ELEMENTS 150
#define EE_PARTICLE_REGENERATIONS 0
#define EE_PARTICLE_ROTATION 0.0f
#define EE_PARTICLE_ZOOM_W 1.0f
#define EE_PARTICLE_ZOOM_H 1.0f
#define EE_PARTICLE_FORCE_V 1.0
#define EE_PARTICLE_FORCE_H 0
#define EE_PARTICLE_TSTEP 100
typedef struct sforce {
    int vertical, horizontal;
} sforce;
typedef struct sforces {
    float angular;
    sforce force, gravity;
} sforces;
typedef struct slife {
    time_t ttl, ttu[EPOSITION_NULL], maxttu, regenerations;
} slife;
typedef struct sdestination {
    int overflow[EPOSITION_NULL];
    float zoom[EPOSITION_NULL];
} sdestination;
typedef struct sparticle {
    scolor color;
    sdestination position;
    slife life;
    sforces forces;
    sforce maximum;
    float rotation, factor[EPOSITION_NULL];
    ctimer stopwatch;
    unsigned int jump; /* color jump */
    bool expired;
} sparticle;
class cparticle:public ccontext {
private:
    ctarga *master;
    carray<sparticle> particles;
    size_t elements;
public:
    sdestination position[EE_PARTICLE_RANGE];
    slife life[EE_PARTICLE_RANGE];
    sforces forces[EE_PARTICLE_RANGE];
    scolor color[EE_PARTICLE_RANGE], conclusive;
    inline cparticle (void) { this->init(); };
    void init (void);
    int initialize (ctarga *master, size_t elements);
    int initialize (cparticle *context);
    inline size_t records (void) { return elements; };
protected:
    void generate (sparticle *particle, bool already=false);
    void refresh (sparticle *particle);
public:
    /* parent's virtual redefiniton */
    scolor *pixel (unsigned int positionx, unsigned int positiony);
    GLuint get (void);
    void restore (void);
    void reset (void);
private:
    void unload (sparticle *particle);
public:
    void unload (void);
};
#endif
