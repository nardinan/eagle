/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CCHANNELS_H
#define EE_CCHANNELS_H
#include "carray.h"
#include "cxml.h"
#include "csample.h"
#define EE_NULLAUDIO 0
typedef struct schannel {
    carray<csample> samples;
    bool exclusive;
    unsigned int volume;
} schannel;
class cchannels:public eagleclass {
private:
    carray<schannel> channels;
    schannel *current;
public:
    inline cchannels (void) { this->init(); };
    void init (void);
    int listening (cxml &parameters);
    int add (bool exclusive, unsigned int volume, const char *label);
    int set (const char *label);
    int append (csample *sample, const char *label);
    int play (const char *label);
    int stop (const char *label);
    int stop (void);
    int pause (const char *label);
    bool playing (const char *label);
    bool playing (void);
    void volume (unsigned int value);
    inline unsigned int volume (void) { return (current)?current->volume:EE_NULLAUDIO; };
private:
    void unload (schannel *singleton);
public:
    void unload (void);
};
#endif
