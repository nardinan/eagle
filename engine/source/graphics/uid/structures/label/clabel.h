/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CLABEL_H
#define EE_CLABEL_H
#include "cfilesystem.h"
#include "ccomponent.h"
#include "ctheme.h"
#define EE_LABEL_W 180
#define EE_LABEL_H 20
#define EE_LABEL_INTERLINE 16
#define EE_LABEL_PARAGRAPH 5
#define EE_LABEL_BACKGROUND false
enum elabel {
    ELABEL_BACKGROUND = 0,
    ELABEL_CORNER_NW,
    ELABEL_CORNER_NE,
    ELABEL_CORNER_SW,
    ELABEL_CORNER_SE,
    ELABEL_BORDER_N,
    ELABEL_BORDER_S,
    ELABEL_BORDER_W,
    ELABEL_BORDER_E,
    ELABEL_NULL
};
typedef struct scharacter {
    char character;
    sfont *font;
} scharacter;
class clabel:public ccomponent {
public:
    static cfonts *replacement;
protected:
    carray<scharacter> characters;
    ccontext *interface[ELABEL_NULL]; /* soft reference */
    cfonts *fonts;
    unsigned int overflow[EPOSITION_NULL], viewport[EPOSITION_NULL], current;
public:
    unsigned int size, interline, paragraph;
    inline clabel (void) { this->init(); };
    void init (void);
    int initialize (ctheme *theme);
    void content (const char *value);
    char *content (void);
    inline unsigned int width (void) { return overflow[EPOSITION_WIDTH]; };
    inline unsigned int height (void) { return overflow[EPOSITION_HEIGHT]; };
    inline void view (eposition coordinates, int value) { this->viewport[coordinates] = value; };
    inline int view (eposition coordinates) { return this->viewport[coordinates]; };
protected:
    void rebuild (elabel component);
    void analyzing (void);
    void calculate (void);
public:
    /* parent's virtual redefiniton */
    GLuint get (void);
    void restore (void);
    void forward (skeyboard *keyboard, smouse *mouse, unsigned int positionx, unsigned int positiony);
    void unload (void);
};
#endif
