/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CMEMORY_H
#define EE_CMEMORY_H
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdarg.h>
#include "GLee.h"
#ifndef _WIN32
#include <opengl/glu.h>
#include <openal/alc.h>
#include <opengl/gl.h>
#include <openal/al.h>
#include <glut/glut.h>
#else
#include <gl/glu.h>
#include <al/alc.h>
#include <gl/gl.h>
#include <al/al.h>
#include <gl/glut.h>
#include <gl/glext.h>
#include <windows.h>
#endif
#ifdef EE_EXTENSION
#undef EE_EXTENSION
#endif
#if defined(GL_TEXTURE_RECTANGLE_ARB)
#define EE_EXTENSION GL_TEXTURE_RECTANGLE_ARB
#elif defined(GL_TEXTURE_RECTANGLE_EXT)
#define EE_EXTENSION GL_TEXTURE_RECTANGLE_EXT
#elif defined(GL_TEXTURE_RECTANGLE_NV)
#define EE_EXTENSION GL_TEXTURE_RECTANGLE_NV
#else
#error eagle engine needs GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_RECTANGLE_EXT or GL_TEXTURE_RECTANGLE_NV extension and your graphic card didn't support it
#endif
#define EE_MAX(a,b)\
    (((a)>(b))?(a):(b))
#define EE_MIN(a,b)\
    (((a)>(b))?(b):(a))
#define EE_SWAP(a,b)\
    do{\
        int i=a;\
        a=b;\
        b=i;\
    }while(0)
#define EE_ROUND(a) ((fabsf(a)<1.0f)?0:a)
#define EE_CHECKSUM 0xDEADBEEF
#define EE_OVERFLOW 20
#define EE_OK 1
#define EE_ERROR 0
#define EE_DONE -1
#define EE_STACK_STREAM "eagle.crash.core.dumped"
#define EE_STACK_SIZE 50
typedef struct scoordinates {
    const char *file;
    unsigned int line;
} scoordinates;
typedef struct sallocation {
    void *pointer;
    size_t size;
    struct sallocation *next, *back;
} sallocation;
extern unsigned long int memory;
extern sallocation *allocations;
extern void *eemalloc (size_t size, const char *file, unsigned int line);
extern void *eerealloc (void *pointer, size_t size, const char *file, unsigned int line);
extern void eefree (void *pointer, const char *file, unsigned int line);
//#define EE_DEBUG_PTRS /* enable controlled pointers */
//#define EE_DEBUG_LOGS /* enable user-friendly frontend's log */
#ifdef EE_DEBUG_PTRS
class eagleclass {
public:
    inline void *operator new (size_t size) { return (void *) malloc(size); };
    inline void *operator new (size_t size, const char *file, unsigned int line) { return (void *) eemalloc(size, file, line); };
    inline void operator delete (void *pointer) { if (pointer) eefree(pointer, "delete operator", 0); };
};
#define ememory(fst)\
    do{\
        sallocation *allocs=allocations;\
        scoordinates *coords=NULL;\
        unsigned char *check=NULL;\
        unsigned int corrects=0,errors=0;\
        while(allocs){\
            check=(unsigned char *)(((char*)allocs->pointer)+(size_t)allocs->size);\
            coords=(scoordinates*)(check+EE_OVERFLOW);\
            if(*((unsigned int*)check)==EE_CHECKSUM)\
                corrects++;\
            else\
                errors++;\
            fprintf(fst,"[pointers::informations] %p at %s (row %d) checksum [%s]\n",allocs->pointer,coords->file,coords->line,(*((unsigned int *)check)==EE_CHECKSUM)?"OK":"ERROR");\
            allocs=allocs->next;\
        }\
        fprintf(fst,"[pointers::summary] %u correct pointers && %u damaged pointers\n",corrects,errors);\
        fprintf(fst,"[pointers::summary] %f KBytes of memory unreleased\n",((float)memory/1024.0f));\
    }while(0)
#define emalloc(a)\
    eemalloc(a,__FILE__,__LINE__)
#define erealloc(a,b)\
    eerealloc(a,b,__FILE__,__LINE__)
#define efree(a)\
    eefree(a,__FILE__,__LINE__)
#define enew\
    new(__FILE__,__LINE__)
#define edelete(a)\
    delete(a,__FILE__,__LINE__)
#else
class eagleclass {/* contain nothing */};
#define ememory(fst)\
    {/* do nothing */}
#define emalloc(a)\
    malloc(a)
#define erealloc(a,b)\
    realloc(a,b)
#define efree(a)\
    free(a)
#define enew\
    new
#define edelete(a)\
    delete(a)
#endif
#define ekill(frm)\
    do{\
        fprintf(stderr,"[eagle::kill] (%s|%d) %s\n",__FILE__,__LINE__,frm);\
        exit(1);\
    }while(0)
#endif
