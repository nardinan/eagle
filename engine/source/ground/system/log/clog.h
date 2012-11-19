/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CLOG_H
#define EE_CLOG_H
#include <time.h>
#include "cstring.h"
#ifdef EE_DEBUG_LOGS
#define elog(str,...)\
    do{\
        clog::log(stdout,__FUNCTION__,str,##__VA_ARGS__);\
    }while(0)
#else
#define elog(str,...)\
    do{\
    }while(0)
#endif
class clog:public eagleclass {
public:
    static void log (FILE *stream, const char *function, const char *format, ...);
};
#endif
