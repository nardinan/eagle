/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CBUTTON_H
#define EE_CBUTTON_H
#include "cfilesystem.h"
#include "ccomponent.h"
#include "ctheme.h"
#include "clabel.h"
#define EE_BUTTON_W 180
#define EE_BUTTON_INTERLINE 16
#define EE_BUTTON_PARAGRAPH 0
#define EE_BUTTON_BACKGROUND true
enum ebutton {
    EBUTTON_LEFT = 0,
    EBUTTON_MIDDLE,
    EBUTTON_RIGHT,
    EBUTTON_LEFT_PRESSED,
    EBUTTON_MIDDLE_PRESSED,
    EBUTTON_RIGHT_PRESSED,
    EBUTTON_NULL
};
class cbutton; /* pre-definition */
typedef void (fbuttonhandler)(cbutton *cmp, const char *identifier);
class cbutton:public ccomponent {
protected:
    char identifier[EE_LABEL_SIZE];
    ccontext *interface[EBUTTON_NULL]; /* soft reference */
    fbuttonhandler *handler;
    ccontext *value;
    unsigned int current;
    bool components, pressed;
public:
    inline cbutton (ccontext *context) { this->init(context); };
    void init (ccontext *context);
    int initialize (ctheme *theme);
    void handleizer (const char *identifier, fbuttonhandler *handler);
protected:
    void rebuild (ebutton component);
public:
    inline ccontext *content (void) { return value; };
    /* parent's virtual redefiniton */
    GLuint get (void);
    void restore (void);
    void forward (skeyboard *keyboard, smouse *mouse, unsigned int positionx, unsigned int positiony);
    void unload (void);
};
#endif
