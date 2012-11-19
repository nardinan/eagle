/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Filippo Paolucci
 * EMAIL        : fantic86@alice.it
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EF_CFX
#define EF_CFX
#include <cengine.h>
#include "debug.h"
#define EF_FX_OFFSET 0
#define EF_FX_BASE 0
#define EF_FX_DISTANCE 10
#define EF_FX_SPEED 1.0f
#define EF_FX_ANGLE 90.0f
#define EF_FX_LAYER 1
enum efxtype {
    EFXTYPE_RECT = 0,
    EFXTYPE_CIRCLE,
    EFXTYPE_NULL
};
enum efxposition{
    EFXPOSITION_NOT_FOLLOW = 0,
    EFXPOSITION_FOLLOW,
    EFXPOSITION_NULL
};
typedef struct sfxrect{
    float direction_angle;
    float speed;
    int start_distance;
} sfxrect;
typedef struct sfxcircle{
    float start_angle;
    float rotation_speed;
    int radius;
} sfxcircle;
class cfx:public cmodel {
public:
    efxtype fxtype;
    efxposition fxposition;
    float speed, angle;
    int distance, basex, basey, offsetx, offsety, lifetime_msecs, fxlayer;
public:
    inline cfx (void) { this->init(); };
    /*
    cfx(sfxcircle circle_setting, int startx, int starty);
    cfx(sfxrect rect_setting, int startx, int starty);
     */
    void init();
    int initialize (sdfssegment* datablock, int layer=1);
    int initialize (cmodel* model, bool hard, int layer=1);
    int copyfx (cfx* fx,bool hard);
    int setfx (sfxcircle circle_setting, efxposition fxposition=EFXPOSITION_NOT_FOLLOW, int lifetime_msecs=1000, int offsetx=0, int offsety=0);
    int setfx (sfxrect rect_setting, efxposition fxposition=EFXPOSITION_NOT_FOLLOW, int lifetime_msecs=1000, int offsetx=0, int offsety=0);
    inline void unload (void) {
        cmodel::unload();
    };
protected:
    int setfx (efxposition fxposition, int lifetime_msecs, int offsetx, int offsety);
private:
    bool initialized;
};
#endif
