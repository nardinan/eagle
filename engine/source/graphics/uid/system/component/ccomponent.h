/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CCOMPONENT_H
#define EE_CCOMPONENT_H
#include "ccontext.h"
#include "cfonts.h"
#include "cinput.h"
#define EE_COMPONENT_REBUILDABLE false
#define safesrc(obj,kin)\
    ((obj)?(obj)->source(kin):0)
#define safedst(obj,kin)\
    ((obj)?(obj)->destination(kin):0)
#define complete(str)\
    ((!(str))||(*(str)=='\0'))
class ccomponent:public ccontext {
protected:
    bool selected, visible, background;
public:
    void init (void) {
        ccontext::init();
        rebuildable = EE_COMPONENT_REBUILDABLE;
    }
    virtual void forward (skeyboard *keyboard, smouse *mouse, unsigned int positionx, unsigned int positiony)=0;
    inline scolor *pixel (unsigned int positionx, unsigned int positiony) { return NULL; };
    inline void visibility (bool visible) { this->visible = visible; };
    inline bool visibility (void) { return visible; };
    inline void backgrounded (bool background) { this->background = background; };
    inline bool backgrounded (void) { return background; };
};
#endif
