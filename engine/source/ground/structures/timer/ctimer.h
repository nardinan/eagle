/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CTIMER_H
#define EE_CTIMER_H
#include <sys/time.h>
#include "carray.h"
#define EE_USECS 1000000
typedef struct stime {
    unsigned long secs, usecs;
} stime;
class ctimer:public eagleclass {
private:
    carray<stime> timers;
    unsigned long kvalue;
public:
    inline ctimer (void) { this->init(); };
    inline void init (void) { timers.init(); kvalue = 0; };
    int add (const char *label);
    int set (const char *label);
    int del (const char *label);
    stime get (const char *label);
    void unload (void);
};
#endif
