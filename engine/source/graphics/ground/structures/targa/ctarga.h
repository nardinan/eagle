/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CTARGA_H
#define EE_CTARGA_H
#include "cfilesystem.h"
#include "ccontext.h"
#include "cmath.h"
#define EE_TARGA_PIXEL 4
#define EE_TARGA_MASK 2
enum etgatype { 
    ETGATYPE_COLOR = 0, 
    ETGATYPE_IMAGE,
    ETGATYPE_NULL
};
#pragma pack(push, 1)
typedef struct stgaheader {
    char oversize, type[ETGATYPE_NULL];
    short index, length;
    char bits;
    unsigned short positionx, positiony, width, height;
    char bpp, description;
} stgaheader;
#pragma pack(pop)
class ctarga:public ccontext {
public:
    static sdfssegment *replacement;
protected:
    stgaheader header;
    unsigned char *data, *mask;
    bool returned; /* single datablock trick */
public:
    inline ctarga (void) { this->init(); };
    void init (void);
    int initialize (sdfssegment *datablock);
    int initialize (ctarga *context);
    int initialize (unsigned int width, unsigned int height, scolor color);
    int invisibility (scolor color, unsigned int threshold=0, bool revealing=true);
    int invisibility (unsigned char value, unsigned int positionx, unsigned int positiony, unsigned int radius, bool revealing=true);
protected:
    int alpha (unsigned int alpha=255);
    int texturize (void);
public:
    /* parent's virtual redefiniton */
    scolor *pixel (unsigned int positionx, unsigned int positiony);
    GLuint get (void);
    void restore (void);
    void unload (void);
};
#endif
