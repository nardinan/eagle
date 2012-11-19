/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CWINDOW_H
#define EE_CWINDOW_H
#include "cfilesystem.h"
#include "ccomponent.h"
#include "ctheme.h"
#define EE_WINDOW_W 320
#define EE_WINDOW_H 240
#define EE_WINDOW_BACKGROUND true
enum ewindow {
    EWINDOW_BACKGROUND = 0,
    EWINDOW_CORNER_NW,
    EWINDOW_CORNER_NE,
    EWINDOW_CORNER_SW,
    EWINDOW_CORNER_SE,
    EWINDOW_BORDER_N,
    EWINDOW_BORDER_S,
    EWINDOW_BORDER_W,
    EWINDOW_BORDER_E,
    EWINDOW_BUTTON_CLOSE,
    EWINDOW_NULL,
};
class cwindow:public ccomponent {
private:
    carray<ccomponent> components;
    ccontext *interface[EWINDOW_NULL]; /* soft reference */
    unsigned int current;
    int overflow[EPOSITION_NULL];
public:
    bool anchored;
    inline cwindow (void) { this->init(); };
    void init (void);
    int initialize (ctheme *theme);
    inline int add (ccomponent *component, const char *label) { return components.add(component, label); };
    inline ccomponent *get (const char *label) { return components.get(label); };
private:
    void rebuild (ewindow component);
    void viewport (void);
public:
    /* parent's virtual redefiniton */
    GLuint get (void);
    void restore (void);
    void forward (skeyboard *keyboard, smouse *mouse, unsigned int positionx, unsigned int positiony);
    void unload (void);
};
#endif
