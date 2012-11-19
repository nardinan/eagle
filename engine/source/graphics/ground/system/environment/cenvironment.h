/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CENVIRONMENT_H
#define EE_CENVIRONMENT_H
#include "ccontext.h"
#define erender(obj,tex,crd)\
    do{\
        glPushMatrix();\
        glLoadIdentity();\
        glTranslatef((crd)[EPOSITION_POSITIONX]+((float)(crd)[EPOSITION_WIDTH]/2.0f),(crd)[EPOSITION_POSITIONY]+((float)(crd)[EPOSITION_HEIGHT]/2.0f),0.0f);\
        if ((obj)->flipped[EFLIPDIRECTION_VERTICAL])\
            glRotatef(180.0f,1.0f,0.0f,0.0f);\
        if ((obj)->flipped[EFLIPDIRECTION_HORIZONTAL])\
            glRotatef(180.0f,0.0f,1.0f,0.0f);\
        glRotatef(fmod(((obj))->rotation, 360),0.0f,0.0f,1.0f);\
        glBindTexture(EE_EXTENSION,(tex));\
        glBegin(GL_QUADS);\
        {\
            glTexCoord2i((obj)->source(EPOSITION_POSITIONX, EPOSITIONSUBJECT_FRAME),(obj)->source(EPOSITION_POSITIONY,EPOSITIONSUBJECT_FRAME));\
            glVertex3f(-((float)(crd)[EPOSITION_WIDTH]/2.0f),((float)(crd)[EPOSITION_HEIGHT]/2.0f), 0);\
            glTexCoord2i(((obj)->source(EPOSITION_POSITIONX, EPOSITIONSUBJECT_FRAME)+(obj)->source(EPOSITION_WIDTH,EPOSITIONSUBJECT_FRAME)),(obj)->source(EPOSITION_POSITIONY,EPOSITIONSUBJECT_FRAME));\
            glVertex3f(((float)(crd)[EPOSITION_WIDTH]/2.0f),((float)(crd)[EPOSITION_HEIGHT]/2.0f), 0);\
            glTexCoord2i(((obj)->source(EPOSITION_POSITIONX, EPOSITIONSUBJECT_FRAME)+(obj)->source(EPOSITION_WIDTH,EPOSITIONSUBJECT_FRAME)),((obj)->source(EPOSITION_POSITIONY,EPOSITIONSUBJECT_FRAME)+(obj)->source(EPOSITION_HEIGHT,EPOSITIONSUBJECT_FRAME)));\
            glVertex3f(((float)(crd)[EPOSITION_WIDTH]/2.0f),-((float)(crd)[EPOSITION_HEIGHT]/2.0f), 0);\
            glTexCoord2i((obj)->source(EPOSITION_POSITIONX, EPOSITIONSUBJECT_FRAME),((obj)->source(EPOSITION_POSITIONY,EPOSITIONSUBJECT_FRAME)+(obj)->source(EPOSITION_HEIGHT,EPOSITIONSUBJECT_FRAME)));\
            glVertex3f(-((float)(crd)[EPOSITION_WIDTH]/2.0f),-((float)(crd)[EPOSITION_HEIGHT]/2.0f), 0);\
        }\
        glEnd();\
        glPopMatrix();\
    }while(0)
#define enormalize(vw,zm,rs,crd)\
    do{\
        enormalizesrc(vw,zm,rs,crd);\
        enormalizedst(vw,zm,rs,crd);\
    }while(0)
#define enormalizesrc(vw,zm,rs,crd)\
    do{\
        (crd)[EPOSITION_POSITIONX]-=(vw)[EPOSITION_POSITIONX];\
        (crd)[EPOSITION_POSITIONY]-=(vw)[EPOSITION_POSITIONY];\
        (crd)[EPOSITION_POSITIONX]=((float)(crd)[EPOSITION_POSITIONX]*(zm)[EPOSITION_WIDTH]);\
        (crd)[EPOSITION_POSITIONY]=((float)(crd)[EPOSITION_POSITIONY]*(zm)[EPOSITION_HEIGHT]);\
    }while(0)
#define enormalizedst(vw,zm,rs,crd)\
    do{\
        (crd)[EPOSITION_HEIGHT]=(((crd)[EPOSITION_HEIGHT]*(int)(rs)[EPOSITION_HEIGHT])/(vw)[EPOSITION_HEIGHT]);\
        (crd)[EPOSITION_WIDTH]=(((crd)[EPOSITION_WIDTH]*(int)(rs)[EPOSITION_WIDTH])/(vw)[EPOSITION_WIDTH]);\
    }while(0)
typedef struct scontext {
    unsigned int layer;
    char label[EE_LABEL_SIZE];
    ccontext *context;
    struct scontext *next, *back;
} scontext;
class cenvironment:public eagleclass {
private:
    scontext *environment, *current;
    size_t elements;
public:
    inline cenvironment (void) { this->init(); };
    void init (void);
    int add (ccontext *context, unsigned int layer=0, const char *label=NULL);
    void del (void);
    ccontext *get (const char *label);
    ccontext *get (ccontext *context);
    inline char *label (void) { return (current)?current->label:NULL; };
    inline size_t records (void) { return elements; }
    inline void reset (void) { current = NULL; }
private:
    void next (void);
public:
    ccontext *nextvalue (void);
    char *nextkey (void);
    void unload (void);
};
#endif
