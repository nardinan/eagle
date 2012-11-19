/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CLIGHTS_H
#define EE_CLIGHTS_H
#include "ctarga.h"
#include "cxml.h"
#include "cshadows.h"
#define EE_LIGHT_SIZE 640
#define EE_LIGHT_INTENSITY 640
#define EE_MASK_BACK 0,0,0,255
#define EE_MASK_SPOT 255,255,255,255
#define EE_MASK_BASE 1.0f
typedef struct slight {
    int positionx, positiony;
    unsigned short intensity;
} slight;
class clights:public eagleclass {
friend class cengine;
private:
    carray<slight> lights;
    ctarga *darkness, *light, *total;
    GLuint framebuffer;
public:
    cshadows shadows;
    float base;
    inline clights (void) { this->init(); };
    void init (void);
    int initialize (unsigned int resolutionx, unsigned int resolutiony);
    int add (int positionx, int positiony, const char *label);
    inline slight *get (const char *label) { return lights.get(label); };
    void del (const char *label);
protected:
    void redisplay (int view[], float zoom[], unsigned int resolution[]);
public:
    int listening (cxml &parameters);
protected:
    void unload (sshadow *singleton);
public:
    void unload (void);
};
#endif