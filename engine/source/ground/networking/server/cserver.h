/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CSERVER_H
#define EE_CSERVER_H
#include "cnetworking.h"
class cserver:public eagleclass {
private:
    int stream;
    sockaddr_in address;
#ifdef _WIN32
    WSADATA datasocket;
#endif
public:
    inline cserver (void) { this->init(); };
    inline void init (void) { stream = -1; };
    int initialize (const char *address, const char *port, unsigned int queue);
    ssocket *search (unsigned int sec=0, unsigned int usec=0);
	int sending (ssocket *client, const char *format, ...);
	int receiving (ssocket *client, char **datablock, unsigned int sec=0, unsigned int usec=0);
private:
	int receiving (int stream, char *datablock, size_t size, unsigned int sec, unsigned int usec);
public:
    void unload (ssocket *client);
    void unload (void);
};
#endif
