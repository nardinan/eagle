/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CENDIAN_H
#define EE_CENDIAN_H
#include "carray.h"
#define EE_BIG_ENDIAN 1
#define EE_LITTLE_ENDIAN 0
#define eswap(val)\
    cendian::swap(&val,sizeof(val))
class cendian:public eagleclass {
public:
    static int check (void);
    static void swap (void *data, size_t length);
};
#endif
