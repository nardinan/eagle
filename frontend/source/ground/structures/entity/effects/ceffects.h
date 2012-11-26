/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Filippo Paolucci
 * EMAIL        : fantic86@alice.it
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */

#ifndef EF_EFFECTS
#define EF_EFFECTS
#ifndef _WIN32
#include <engine/cengine.h>
#else
#include <cengine.h>
#endif
#include "cfx.h"
#include "debug.h"
class ceffects:public eagleclass {
public:
    static void init ();
    static int addfx (cfx* fx, const char* label);
    inline static cfx* getfx (const char* label) { return cfxlist->get(label); };
    static int setfx (const char* label, sfxcircle circle_setting, efxposition fxposition, int lifetime_msecs, int offsetx=0, int offsety=0);
    static int setfx (const char* label, sfxrect rect_setting, efxposition fxposition, int lifetime_msecs, int offsetx=0, int offsety=0);
    static int delfx (const char* label);
    inline static void unload (cfx* fx){
        fx->unload();
        delete fx;
    }
    inline static void unload (void) {
        destroy(cfxlist, unload, cfx);
        delete cfxlist;
    }
private:
     static carray<cfx> *cfxlist;
};
#endif
