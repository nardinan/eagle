/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CRANDOM_H
#define EE_CRANDOM_H
#include <stdlib.h>
#define EE_RAND_MAX 2147483647 /* 32 bit random */
#define erandrange(tp,dw,res)\
    do{\
        res=((abs(((int)tp)-((int)dw))));\
        if(res>0)\
            res=(erand()%(res+1));\
        res+=EE_MIN((tp),(dw));\
    }while(0)
#define efrandrange(tp,dw,res)\
    do{\
        res=(fabsf(((float)tp)-((float)dw)));\
        if(res>0.0f)\
            res=fmod(((float)erand()/(float)EE_RAND_MAX),res);\
        res+=EE_MIN((tp),(dw));\
    }while(0)
#define estep(bec,val,stp)\
    do{\
        if((bec)>(val))\
            (val)+=(stp);\
        if((bec)<(val))\
            (val)-=(stp);\
    }while(0)
extern int erand (void);
extern void esrand (unsigned int seed);
#endif
