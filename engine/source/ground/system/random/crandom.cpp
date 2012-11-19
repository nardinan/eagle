/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "crandom.h"
#define EE_RANDOM_Z_A 36969
#define EE_RANDOM_Z_B 65535
#define EE_RANDOM_Z_BITS 16
#define EE_RANDOM_W_A 18000
#define EE_RANDOM_W_B 65535
#define EE_RANDOM_W_BITS 16
#define EE_RANDOM_MWC_BITS 16
#define EE_RANDOM_ZNEW (ez=EE_RANDOM_Z_A*(ez&EE_RANDOM_Z_B)+(ez>>EE_RANDOM_Z_BITS))
#define EE_RANDOM_WNEW (ew=EE_RANDOM_W_A*(ew&EE_RANDOM_W_B)+(ew>>EE_RANDOM_W_BITS))
#define EE_RANDOM_MWC ((EE_RANDOM_ZNEW<<EE_RANDOM_MWC_BITS)+EE_RANDOM_WNEW)
static unsigned long ez=362436069, ew=521288629;
int erand (void) {
    return (int)abs((long)EE_RANDOM_MWC);
}

void esrand (unsigned int seed) {
    srand(seed);
    ew = seed;
    ez = EE_RANDOM_WNEW;
    ew = EE_RANDOM_ZNEW;
}
