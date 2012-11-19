/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CLIST_H
#define EE_CLIST_H
#include "clabel.h"
#define EE_LIST_BACKGROUND true
class clist; /* pre-definition */
typedef void (flisthandler)(clist *cmp, const char *identifier, const char *selected);
class clist:public clabel {
protected:
    char identifier[EE_LABEL_SIZE], choosed[EE_LABEL_SIZE];
    flisthandler *handler;
public:
    inline clist (void) { this->init(); };
    void init (void);
    void handleizer (const char *identifier, flisthandler *handler);
    void forward (skeyboard *keyboard, smouse *mouse, unsigned int positionx, unsigned int positiony);
protected:
    int search (unsigned int positionx, unsigned int positiony);
};
#endif
