/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CCLIENT_H
#define EE_CCLIENT_H
#include "cnetworking.h"
class cclient:public eagleclass {
private:
    ssocket infos;
#ifdef _WIN32
    WSADATA datasocket;
#endif
public:
    inline cclient (void) { this->init(); };
    inline void init (void) { infos.stream = -1; };
    int initialize (const char *address, const char *port);
	int sending (const char *format, ...);
	int receiving (char **datablock, unsigned int sec=0, unsigned int usec=0);
private:
	int receiving (int stream, char *datablock, size_t size, unsigned int sec, unsigned int usec);
public:
    void unload (void);
};
#endif
