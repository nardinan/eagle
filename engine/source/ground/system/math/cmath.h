/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CMATH_H
#define EE_CMATH_H
#include <math.h>
#include "cmemory.h"
class cmath:public eagleclass {
public:
    static unsigned int npow2 (unsigned int value);
    static float radtodegree, degreetorad, ecosa[], esina[], pi;
    static float sqrt (float value);
    static float pow2 (float base);
    static float pow3 (float base);
};
#endif
