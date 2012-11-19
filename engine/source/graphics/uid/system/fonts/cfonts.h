/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CFONTS_H
#define EE_CFONTS_H
#include "ctarga.h"
#define EE_CHARACTERS 256
#define pink(pxl)\
    ((pxl->red==255)&&(pxl->green==0)&&(pxl->blue==255))
typedef struct sfont {
    ccontext *layer;
    unsigned int positions[EE_CHARACTERS/2][EPOSITION_NULL], offset[EE_CHARACTERS], last, space;
    struct {
        float color[ECOLOR_NULL];
    } channels;
} sfont;
class cfonts:public eagleclass {
public:
    static ccontext *replacement;
private:
    carray<sfont> fonts;
    unsigned int height, width;
public:
    inline cfonts (void) { this->init(); };
    void init (void);
    int add (ccontext *layer, const char *label);
private:
    int analyze (sfont *font);
public:
    void del (const char *label);
    inline sfont *get (const char *label) { return fonts.get(label); };
    void unload (void);
};
#endif
