/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CLOBBY_H
#define EE_CLOBBY_H
#include "cfilesystem.h"
#include "cxml.h"
#include "cserver.h"
#define EE_LOBBY_LIMIT 8
#define EE_LOBBY_QUEUE 4
#define EE_LOBBY_ADDRESS "127.0.0.1"
#define EE_LOBBY_PORT "5090"
#define safecond(con,loc,res)\
    do{\
        pthread_mutex_lock(&(loc));\
        if((con))\
            (res)=true;\
        else\
            (res)=false;\
        pthread_mutex_unlock(&(loc));\
    }while(0)
#define safefunction(ope,loc,res)\
    do{\
        pthread_mutex_lock(&(loc));\
        (res)=(ope);\
        pthread_mutex_unlock(&(loc));\
    }while(0)
#define safemethod(ope,loc)\
    do{\
        pthread_mutex_lock(&(loc));\
        (ope);\
        pthread_mutex_unlock(&(loc));\
    }while(0)
enum elobbyconfiguration {
    ELOBBYCONFIGURATION_ADDRESS = 0,
    ELOBBYCONFIGURATION_PORT,
    ELOBBYCONFIGURATION_NULL
};
typedef void (flobbyinstance)(cserver *manager, ssocket *channel);
typedef struct slobbyconfiguration {
    cserver *server;
    unsigned int queue, limit;
    flobbyinstance *reference;
} slobbyconfiguration;
typedef struct sclientconfiguration {
    cserver *server;
    ssocket *socket;
    flobbyinstance *reference;
} sclientconfiguration;
class clobby:public eagleclass {
public:
    static unsigned int clients;
private:
    static pthread_mutex_t mutex;
public:
    static int listening (cxml &lobby, flobbyinstance *reference);
private:
    static void *manage (void *parameters);
    static void *connect (void *parameters);
};
#endif
