/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CSCROLLER_H
#define EE_CSCROLLER_H
#include "clabel.h"
#define EE_SCROLLER_MOVEMENT 21
#define EE_SCROLLER_SPACE 1024
#define EE_SCROLLER_BACKGROUND true
enum escroller {
    ESCROLLER_ARROW_UP,
    ESCROLLER_ARROW_MIDDLE,
    ESCROLLER_ARROW_DOWN,
    ESCROLLER_SCROLLER,
    ESCROLLER_NULL
};
class cscroller:public ccomponent {
protected:
    ccontext *interface[ESCROLLER_NULL]; /* soft reference */
    clabel *value;
    unsigned int current;
    bool components;
public:
    unsigned int increment, space;
    inline cscroller (clabel *component) { this->init(component); };
    void init (clabel *component);
    int initialize (ctheme *theme);
protected:
    void rebuild (escroller component);
public:
    /* parent's virtual redefiniton */
    GLuint get (void);
    void restore (void);
    void forward (skeyboard *keyboard, smouse *mouse, unsigned int positionx, unsigned int positiony);
    inline void unload (void) {}; /* do nothing, just redefine the virtual method */
};
#endif
