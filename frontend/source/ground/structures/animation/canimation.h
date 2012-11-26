/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Filippo Paolucci
 * EMAIL        : fantic86@alice.it
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EF_ANIMATION
#define EF_ANIMATION
#ifndef _WIN32
#include <engine/cengine.h>
#else
#include <cengine.h>
#endif
#include "cfx.h"
#include "debug.h"
class canimation:public eagleclass {
public:
    static int loadmodel (cmodel** modelptr, const char* packagefilename);
    static int loadmodel (cfx** modelptr, const char* packagefilename);
    static int loadmodel (ctarga** modelptr, const char* packagefilename);
    static int setmodel (cmodel* model, const char* movementlabel );
    static int fileexist (const char* filename );
};
#endif
