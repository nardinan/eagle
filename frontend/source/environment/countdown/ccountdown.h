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
#ifndef _WIN32
#include <engine/cengine.h>
#else
#include <cengine.h>
#endif
#include "debug.h"
class ccountdown:public ctimer{
public:
    inline ccountdown (void) { this->init(); };
    int check (const char* label, int countdown_ms);
};
#endif


