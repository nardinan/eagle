/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CXML_H
#define EE_CXML_H
#include "cfilesystem.h"
#define EE_TOKEN_SIZE 256
#define flagsassign(flg,num,val)\
    do{\
        int ind;\
        for(ind=0;ind<num;ind++)\
            flg[ind].symbols=flg[ind].special=flg[ind].characters=flg[ind].safe=val;\
    }while(0)
#define flagscheck(flg,num,val)\
    do{\
        int ind;\
        val=true;\
        for(ind=1;ind<num;ind++)\
            if((flg[ind-1].symbols!=flg[ind].symbols)||(flg[ind-1].special!=flg[ind].special)||(flg[ind-1].characters!=flg[ind].characters)){\
                val=false;\
                break;\
            }\
    }while(0)
enum exmlaction {
    EXMLACTION_READ,
    EXMLACTION_TITLE,
    EXMLACTION_KEY,
    EXMLACTION_KVALUE,
    EXMLACTION_CVALUE,
    EXMLACTION_JUMP 
};
typedef struct sxmlflags {
    bool symbols, special, characters, safe;
} sxmlflags;
typedef struct sxmlcontroller {
    bool opened, closed;
} sxmlcontroller;
typedef struct sxmltoken {
    char token[EE_TOKEN_SIZE];
    int length;
} sxmltoken;
typedef struct sxmlclass {
    carray<sxmlclass> subclass;
    carray<char> keys;
    char *value, label[EE_LABEL_SIZE];
    struct sxmlclass *back;
} sxmlclass;
class cxml:public eagleclass {
private:
    sxmlclass *structure, *pointer;
public:
    inline cxml (void) { this->init(); };
    void init (void);
    int analyze (sdfssegment *datablock);
    inline int draw (FILE *stream) { return this->draw(stream, structure, 0); };
private:
    int analyze (carray<sxmltoken> *tokens);
    int draw (FILE *stream, sxmlclass *value, int deep);
public:
    int addkey (const char *value, const char *label);
    int addvalue (const char *value);
    int addclass (const char *value);
    sxmlclass *get (const char *format, ...);
private:
    sxmlclass *vaget (const char *format, va_list args);
    void unload (sxmlclass *value);
public:
    static carray<sxmltoken> *xmltokenizer (sdfssegment *datablock);
    inline void unload (void) { unload(structure); structure = NULL; pointer = NULL; };
};
#endif
