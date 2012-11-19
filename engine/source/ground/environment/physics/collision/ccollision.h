/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CCOLLISION_H
#define EE_CCOLLISION_H
#include "ccontext.h"
#include "cmath.h"
#define EE_RECTANGLE_TWO 0
#define EE_RECTANGLE_ONE 1
#define EE_RECTANGLES 2
#define coordx(ln,val)\
    ln.point[val].positionx
#define coordy(ln,val)\
    ln.point[val].positiony
enum eaxis {
    EAXIS_TOP,
    EAXIS_BOTTOM,
    EAXIS_RIGHT,
    EAXIS_LEFT,
    EAXIS_NULL
};
enum ecorners {
    ECORNER_TOPLEFT,
    ECORNER_TOPRIGHT,
    ECORNER_BOTTOMLEFT,
    ECORNER_BOTTOMRIGHT,
    ECORNER_NULL
};
enum edirection {
    EDIRECTION_TOP = 1,
    EDIRECTION_BOTTOM = 2,
    EDIRECTION_RIGHT = 4,
    EDIRECTION_TOPRIGHT = 5,
    EDIRECTION_BOTTOMRIGHT = 6,
    EDIRECTION_LEFT = 8,
    EDIRECTION_TOPLEFT = 9,
    EDIRECTION_BOTTOMLEFT = 10,
    EDIRECTION_NULL = 0
};
typedef struct spoint {
    int positionx, positiony;
} spoint;
typedef struct sline {
    spoint point[2];
} sline;
typedef struct srectangle {
    spoint origin;
    unsigned int width, height;
    float angle;
    spoint pivot;
} srectangle;
typedef struct scollision {
    spoint impact;
    bool collision;
} scollision;
typedef struct sdirection {
    unsigned int vertical, horizontal;
    edirection direction;
} sdirection;
class ccollision:public eagleclass {
public:
    static scollision collision (srectangle collider, srectangle agent);
    static scollision collision (ccontext *collider, ccontext *agent);
    static spoint rotate (int positionx, int positiony, spoint pivot, float sina, float cosa);
    static spoint rotate (spoint point, spoint pivot, float sina, float cosa);
    static spoint pivot (int positionx, int positiony, int width, int height);
    static spoint pivot (ccontext *context);
private:
    static sline axis (eaxis target, int positionx, int positiony, int width, int height, float sina, float cosa, spoint pivot);
    static scollision intersect (sline collider, sline other);
public:
    static float distance (spoint source, spoint destination);
    static float distance (ccontext *source, ccontext *destination);
    static sdirection position (spoint origin, spoint target);
};
#endif
