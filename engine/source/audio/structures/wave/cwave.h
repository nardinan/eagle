/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CWAVE_H
#define EE_CWAVE_H
#include "csample.h"
#define EE_WAVE_DESCRIPTOR 4
#define check3(id,ck)\
    (((id)[0]==(ck)[0])&&((id)[1]==(ck)[1])&&((id)[2]==(ck)[2]))
#define check4(id,ck)\
    (((id)[0]==(ck)[0])&&((id)[1]==(ck)[1])&&((id)[2]==(ck)[2])&&((id)[3]==(ck)[3]))
#pragma pack(push, 1)
typedef struct schunkheader {
    char ID[EE_WAVE_DESCRIPTOR];
    int size;
} schunkheader;
typedef struct sfmtheader {
    short format, channels;
    int sample, byte;
    short align, bps, extra;
} sfmtheader;
typedef struct swavheader {
    char descriptor[EE_WAVE_DESCRIPTOR];
    int size;
    char format[EE_WAVE_DESCRIPTOR];
} swavheader;
#pragma pack(pop)
class cwave:public csample {
protected:
    swavheader header;
    sfmtheader informations;
    int size;
    unsigned char *data;
public:
    inline cwave (void) { this->init(); };
    void init (void);
    int initialize (sdfssegment *datablock, bool loop);
    void play (void);
    void stop (void);
    void pause (void);
    bool playing (void);
    void volume (unsigned int value);
protected:
    static inline ALenum format (short channels, short bps) { return (channels==1)?AL_FORMAT_MONO16:AL_FORMAT_STEREO16; };
    int sampleizer (bool loop);
public:
    void unload (void);
};
#endif
