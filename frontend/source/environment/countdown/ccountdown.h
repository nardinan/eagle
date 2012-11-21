/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Filippo Paolucci
 * EMAIL        : fantic86@alice.it
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */

#ifndef EF_COUNTDOWN
#define EF_COUNTDOWN
#include "cengine.h"
#include "debug.h"
#ifdef EE_CCOUNTDOWN_PRINT
#undef EE_CCOUNTDOWN_PRINT
#define EE_CCOUNTDOWN_PRINT 1
#else 
#define EE_CCOUNTDOWN_PRINT 0
#endif
class ccountdown:public ctimer{
public:
    inline ccountdown () { this->init(); };
    int check (const char* label, int countdown_ms);
};
#endif


