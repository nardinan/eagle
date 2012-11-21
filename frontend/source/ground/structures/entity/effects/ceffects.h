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
#include "cengine.h"
#include "cstring.h"
#include "cfx.h"
#include "debug.h"
#ifdef EE_CEFFECTS_PRINT
#undef EE_CEFFECTS_PRINT
#define EE_CEFFECTS_PRINT 1
#else 
#define EE_CEFFECTS_PRINT 0
#endif
class ceffects:public eagleclass {
public:
    static void init ();
    static int addfx(cfx* fx, const char* label);
    static cfx* getfx (const char* label);
    static int setfx(const char* label, sfxcircle circle_setting, efxposition fxposition,int lifetime_msecs, int offsetx=0, int offsety=0);
    static int setfx(const char* label, sfxrect rect_setting, efxposition fxposition, int lifetime_msecs, int offsetx=0, int offsety=0);
    static int delfx(const char* label);
    static sfxcircle getcircle_setting(float start_angle=90.0f,float rotation_speed=1.0f , int radius=10)  ;
    static sfxrect getrect_setting(float direction_angle=90.0f,float speed=1.0f , int start_distance=10) ;
    inline static void unload(cfx* fx){
        fx->unload();
        delete fx;
    }
    inline static void unload() {
        destroy(cfxlist, unload, cfx);
        delete cfxlist;
    }
private:
     static carray<cfx> *cfxlist;
};
#endif


