/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CFIELD_H
#define EE_CFIELD_H
#include "clabel.h"
#define EE_FIELD_SIZE 256
#define EE_FIELD_BACKGROUND true
#define EE_FIELD_MULTILINE true
#define EE_FIELD_EDITABLE true
#define EE_CARET_LAST (int)((((unsigned int)-1)>>1)+1)
class cfield:public clabel {
protected:
    bool selected;
public:
    bool multiline, editable;
    inline cfield (void) { this->init(); };
    void init (void);
    void forward (skeyboard *keyboard, smouse *mouse, unsigned int positionx, unsigned int positiony);
};
#endif
