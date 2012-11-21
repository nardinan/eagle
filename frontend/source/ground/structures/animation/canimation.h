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
#include "cengine.h"
#include "cfx.h"
#include "debug.h"
#ifdef EE_CANIMATION_PRINT
#undef EE_CANIMATION_PRINT
#define EE_CANIMATION_PRINT 1
#else 
#define EE_CANIMATION_PRINT 0
#endif
class canimation:public eagleclass {
public:
    static int loadmodel (cmodel** modelptr, const char* packagefilename);
    static int loadmodel (cfx** modelptr, const char* packagefilename);
    static int loadmodel (ctarga** modelptr, const char* packagefilename);
    static int setmodel (cmodel* model, const char* movementlabel );
    static int fileexist (const char* filename );
};
#endif