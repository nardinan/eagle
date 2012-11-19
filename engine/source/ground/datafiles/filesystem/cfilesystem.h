/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CFILESYSTEM_H
#define EE_CFILESYSTEM_H
#include "cstring.h"
#include "carray.h"
#include "cendian.h"
#define EE_DFS "A2PN"
#pragma pack(push, 1)
typedef struct sdfsheader {
    char label[EE_LABEL_SIZE];
    size_t size;
} sdfsheader;
#pragma pack(pop)
typedef struct sdfssegment {
    sdfsheader header;
    unsigned char *datablock;
} sdfssegment;
class cfilesystem:public eagleclass {
private:
    sdfsheader header;
    carray<sdfssegment> segments;
public:
    inline cfilesystem (void) { this->init(); };
    inline void init (void) { segments.init(); };
    int initialize (sdfssegment *datablock);
    int initialize (const char *filename);
    int add (const char *filename, const char *label);  
    int del (const char *label);
    inline sdfssegment *get (const char *label) { return segments.get(label); };
    int collapse (const char *filename);
private:
    void unload (sdfssegment *singleton);
public:
    void unload (void);
};
#endif
