/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CBROWSER_H
#define EE_CBROWSER_H
#include <dirent.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/types.h>
#include "clist.h"
#define EE_BROWSER_BACKGROUND true
#define EE_BROWSER_SIZE 512
extern char pathcharacter;
extern char pathseparator[];
extern char pathroot[];
enum efilter {
    EFILTER_SPECIALS,
    EFILTER_HIDDENS,
    EFILTER_DIRECTORIES,
    EFILTER_FILES,
    EFILTER_LINKS,
    EFILTER_NULL
};
class cbrowser; /* pre-definition */
typedef void (fbrowserhandler)(cbrowser *cmp, const char *identifier, const char *abspath, const char *selected);
class cbrowser:public clist {
protected:
    char abspath[EE_BROWSER_SIZE];
    fbrowserhandler *handler;
public:
    bool filter[EFILTER_NULL];
    inline cbrowser (const char *startpoint) { this->init(startpoint); };
    void init (const char *startpoint);
    void content (const char *value);
    void handleizer (const char *identifier, fbrowserhandler *handler);
    void forward (skeyboard *keyboard, smouse *mouse, unsigned int positionx, unsigned int positiony);
    char *content (void);
    void refresh (void);
};
#endif
