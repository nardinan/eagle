/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CCONTEXT_H
#define EE_CCONTEXT_H
#include <math.h>
#include "carray.h"
#define R(x) (x)
#define G(x) (x)+1
#define B(x) (x)+2
#define A(x) (x)+3
#define S(x) (x)
#define T(x) (x)+1
#define EE_NULLTEXTURE 0
#define EE_CONTEXT_ROTATION 0.0
#define EE_CONTEXT_REBUILDABLE true
#define singletexture(x,obj)\
    while((((x)=(obj)->get()) == EE_NULLTEXTURE));
#define setx(obj,val)\
    (obj)->drop(EPOSITION_POSITIONX,val)
#define sety(obj,val)\
    (obj)->drop(EPOSITION_POSITIONY,val)
#define setw(obj,val)\
    (obj)->drop(EPOSITION_WIDTH,val)
#define seth(obj,val)\
    (obj)->drop(EPOSITION_HEIGHT,val)
#define setr(obj,val)\
    (obj)->masking(ECOLOR_R,val)
#define setg(obj,val)\
    (obj)->masking(ECOLOR_G,val)
#define setb(obj,val)\
    (obj)->masking(ECOLOR_B,val)
#define seta(obj,val)\
    (obj)->masking(ECOLOR_A,val)
#define getx(obj)\
    (obj)->destination(EPOSITION_POSITIONX)
#define gety(obj)\
    (obj)->destination(EPOSITION_POSITIONY)
#define getw(obj)\
    (obj)->destination(EPOSITION_WIDTH)
#define geth(obj)\
    (obj)->destination(EPOSITION_HEIGHT)
#define getr(obj)\
    (obj)->gloss(ECOLOR_R)
#define getg(obj)\
    (obj)->gloss(ECOLOR_G)
#define getb(obj)\
    (obj)->gloss(ECOLOR_B)
#define geta(obj)\
    (obj)->gloss(ECOLOR_A)
#define mastercolor(chl,mas) EE_MIN(EE_MAX(0.0f,((mas)-(1.0f-(chl)))),1.0f)
enum eflipdirection {
    EFLIPDIRECTION_VERTICAL = 0,
    EFLIPDIRECTION_HORIZONTAL,
    EFLIPDIRECTION_NULL
};
enum epositionsubject {
    EPOSITIONSUBJECT_FRAME = 0,
    EPOSITIONSUBJECT_ELEMENT,
    EPOSITIONSUBJECT_BEFORE,
    EPOSITIONSUBJECT_NULL
};
enum eposition {
    EPOSITION_POSITIONX = 0,
    EPOSITION_POSITIONY,
    EPOSITION_WIDTH,
    EPOSITION_HEIGHT,
    EPOSITION_NULL
};
enum ecolor {
    ECOLOR_R = 0,
    ECOLOR_G,
    ECOLOR_B,
    ECOLOR_A,
    ECOLOR_NULL
};
#pragma pack(push, 1)
typedef struct scolor {
    unsigned char red, green, blue, alpha;
} scolor;
#pragma pack(pop)
typedef struct sposition {
    int source[EPOSITION_NULL], destination[EPOSITION_NULL];
} sposition;
/* every graphic component has ccontext as super class */
class ccontext:public eagleclass {
friend class cengine;
protected:
    sposition coordinates[EPOSITIONSUBJECT_NULL];
    struct {
        float color[ECOLOR_NULL];
    } channels[EPOSITIONSUBJECT_NULL];
    GLuint texture;
    bool rebuildable;
public:
    float rotation;
    bool flipped[EFLIPDIRECTION_NULL], subpixel;
    inline ccontext (void) { this->init(); }
    void init (void) {
        coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_POSITIONX] = coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_POSITIONY] = 0;
        coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_POSITIONX] = coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_POSITIONY] = 0;
        coordinates[EPOSITIONSUBJECT_ELEMENT].destination[EPOSITION_POSITIONX] = coordinates[EPOSITIONSUBJECT_ELEMENT].destination[EPOSITION_POSITIONY] = 0;
        coordinates[EPOSITIONSUBJECT_ELEMENT].source[EPOSITION_POSITIONX] = coordinates[EPOSITIONSUBJECT_ELEMENT].source[EPOSITION_POSITIONY] = 0;
        coordinates[EPOSITIONSUBJECT_BEFORE].destination[EPOSITION_POSITIONX] = coordinates[EPOSITIONSUBJECT_BEFORE].destination[EPOSITION_POSITIONY] = 0;
        coordinates[EPOSITIONSUBJECT_BEFORE].source[EPOSITION_POSITIONX] = coordinates[EPOSITIONSUBJECT_BEFORE].source[EPOSITION_POSITIONY] = 0;
        channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_R] = channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_G] = channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_B] = channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_A] = 1.0f;
        channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_R] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_G] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_B] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_A] = 1.0f;
        texture = EE_NULLTEXTURE;
        rotation = EE_CONTEXT_ROTATION;
        rebuildable = EE_CONTEXT_REBUILDABLE;
        flipped[EFLIPDIRECTION_HORIZONTAL] = flipped[EFLIPDIRECTION_VERTICAL] = false;
        subpixel = false;
    }
    inline int source (eposition coordinates, epositionsubject subject=EPOSITIONSUBJECT_ELEMENT) { return this->coordinates[subject].source[coordinates]; };
    inline int destination (eposition coordinates, epositionsubject subject=EPOSITIONSUBJECT_ELEMENT) { return this->coordinates[subject].destination[coordinates]; };
    inline float gloss (ecolor color, epositionsubject subject=EPOSITIONSUBJECT_ELEMENT) { return channels[subject].color[color]; };
    inline void crop (eposition coordinates, int value) { this->coordinates[EPOSITIONSUBJECT_ELEMENT].source[coordinates] = value; };
    inline void drop (eposition coordinates, int value) { this->coordinates[EPOSITIONSUBJECT_ELEMENT].destination[coordinates] = value; };
    inline void masking (ecolor color, float value) { channels[EPOSITIONSUBJECT_ELEMENT].color[color] = EE_MIN(EE_MAX(value, 0.0f),1.0f); };
    inline void viewported (bool rebuildable) { this->rebuildable = rebuildable; };
    inline bool viewported (void) { return this->rebuildable; };
    virtual scolor *pixel (unsigned int positionx, unsigned int positiony)=0;
    virtual GLuint get (void)=0;
    virtual void restore (void)=0;
    virtual void unload (void)=0;
};
#endif
