/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CSTRING_H
#define EE_CSTRING_H
#include "crandom.h"
#include "carray.h"
#define EE_DIGITS_SIZE 15
#define EE_DICTIONARY_SIZE 360
#define EE_ASCII_MAX 127
#define EE_ASCII_MIN 31
#define finalchar(x)\
    (((x)=='\n')||((x)=='\r')||((x)=='\0'))
#define spacechar(x)\
    (((x)=='\t')||((x)==' '))
#define realmchar(x)\
    (((x)!='\t')&&((x)!='\n')&&((x)!=' '))
#define constchar(x)\
    ((((x)>=32)&&((x)<=126))||((x)==9)||((x)==10))
#define strapp(str,app)\
    do{\
        size_t strsiz=estrlen(str),appsiz=estrlen(app),tot;\
        tot=strsiz+appsiz;\
        if (((str)=(char*) erealloc((str),tot+1)))\
            strcpy((str)+(strsiz),(app));\
    }while(0)
#define strand(str,len)\
    do {\
        size_t i,res;\
        for (i=0;i<len;i++){\
            do {\
                res=(erand()%(122-48))+48;\
            } while (((res>57)&&(res<65))||((res>90)&&(res<95))||((res>95)&&(res<97)));\
            str[i]=(char)res;\
        }\
        str[i]='\0';\
    } while(0)
#define eatoi(val)\
    ((val)?atoi(val):0)
#define eatof(val)\
    ((val)?atof(val):0.0f)
class cstring:public eagleclass {
public:
    static char *nearchar (const char *string, const char *dividers);
    static carray<char> *explode (const char *string, const char *dividers, int limit=-1);
    static int defrag (const char *string, const char *dividers, const char *format, ...);
    static int trim (char **string);
    static int compress (char **string, const char *dictionary);
    static int decompress (char **string, const char *dictionary);
};
#endif
