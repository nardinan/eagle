/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CSAMPLE_H
#define EE_CSAMPLE_H
#include "carray.h"
#include "cfilesystem.h"
#define EE_NULLSAMPLE 0
#define EE_SAMPLE_VOLUME 255
class csample:public eagleclass {
protected:
    ALuint sample, buffer;
    int audio;
public:
    inline csample (void) { this->init(); };
    void init (void) {
        sample = buffer = EE_NULLSAMPLE;
        audio = EE_SAMPLE_VOLUME;
    }
    int initialize (sdfssegment *datablock, bool loop);
    virtual void play (void)=0;
    virtual void stop (void)=0;
    virtual void pause (void)=0;
    virtual bool playing (void)=0;
    virtual void volume (unsigned int value)=0;
    unsigned int volume (void) { return audio; };
    virtual void unload (void)=0;
};
#endif
