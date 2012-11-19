/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "cclient.h"
int cclient::initialize (const char *address, const char *port) {
    struct addrinfo hints = {AI_PASSIVE, AF_INET, SOCK_STREAM}, *response = NULL;
	int result = EE_OK, flags, value = 1;
    unsigned long mode = 1;
#ifdef _WIN32
	WSAStartup(MAKEWORD(1,1), &datasocket);
#endif
	if (getaddrinfo(address, port, &hints, &response) == 0) {
		if ((infos.stream = socket(response->ai_family, response->ai_socktype, response->ai_protocol)) > -1) {
			if (connect(infos.stream, response->ai_addr, response->ai_addrlen) == 0) {
#ifndef _WIN32
                mode = 0; /* warning avoid */
                if ((flags = fcntl(infos.stream, F_GETFL)) != -1)
                    fcntl(infos.stream, F_SETFL, flags|O_NONBLOCK);
#else
                ioctlsocket(infos.stream, FIONBIO, &mode);
#endif
#ifdef SO_NOSIGPIPE
                setsockopt(infos.stream, SOL_SOCKET, SO_NOSIGPIPE, (char *)&value, sizeof(int));
#endif
                infos.action = ENETWORKACTION_HEADER;
                infos.length = 0;
			} else
                result = EE_ERROR;
		} else
            result = EE_ERROR;
		freeaddrinfo(response);
	}
	return result;
}

int cclient::sending (const char *format, ...) {
    va_list args;
	char datablock[EE_CONTENT_SIZE], introduction[EE_HEADER_SIZE] = EE_HEADER;
    int index, result = EE_OK;
	size_t length, shifted;
	va_start(args, format);
    {
        vsnprintf(datablock, EE_CONTENT_SIZE, format, args);
        length = shifted = estrlen(datablock);
        if (cendian::check() == EE_BIG_ENDIAN)
            eswap(shifted);
        for (index = EE_BUFFER_SIZE; index < EE_HEADER_SIZE; index++)
            introduction[index] = (shifted >> (8*(index-EE_BUFFER_SIZE)))&0xff;
        result = esocketsend(infos.stream, introduction, datablock, length);
    }
	va_end(args);
	return result;
}

int cclient::receiving (char **datablock, unsigned int sec, unsigned int usec) {
    fd_set rdset;
	char *pointer, introduction[EE_HEADER_SIZE], parser[EE_BUFFER_SIZE] = EE_HEADER;
	struct timeval timeout = {(int)sec, (int)usec};
    int index, result = EE_OK;
    FD_ZERO(&rdset);
    FD_SET(infos.stream, &rdset);
    if (select(infos.stream+1, &rdset, NULL, NULL, (((timeout.tv_sec==0)&&(timeout.tv_usec==0))?NULL:&timeout)) > 0) {
        switch (infos.action) {
            case ENETWORKACTION_HEADER:
                if ((result = receiving(infos.stream, (char *)introduction, EE_HEADER_SIZE, 0, 1)) == EE_OK) {
                    pointer = (&introduction[EE_BUFFER_SIZE]);
                    for (index = 0; index < EE_BUFFER_SIZE; index++)
                        if (introduction[index] != parser[index])
                            break;
                    if (index == EE_BUFFER_SIZE) {
                        memcpy(&infos.length, pointer, sizeof(size_t));
                        if (cendian::check() == EE_BIG_ENDIAN)
                            eswap(infos.length);
                        infos.action = ENETWORKACTION_BODY;
                    } /* skipping corrupted header */
                }
                break;
            case ENETWORKACTION_BODY:
                if ((*datablock = (char *) emalloc(infos.length+1))) {
                    if (receiving(infos.stream, *datablock, infos.length, 0, 1) != EE_OK) {
                        efree(*datablock);
                        *datablock = NULL;
                        result = EE_ERROR;
                    }
                    infos.action = ENETWORKACTION_HEADER;                            
                } else
                    ekill("out of memory");
                break;
        }
    }
	return result;
}

int cclient::receiving (int stream, char *datablock, size_t size, unsigned int sec, unsigned int usec) {
    fd_set rdset;
	char *pointer = datablock;
	ssize_t length = EAGAIN;
    struct timeval timeout = {(int)sec, (int)usec};
	int result = EE_OK;
	FD_ZERO(&rdset);
	FD_SET(stream, &rdset);
	while ((size > 0) && (length == EAGAIN)) {
        if (select(stream+1, &rdset, NULL, NULL,(((timeout.tv_sec==0)&&(timeout.tv_usec==0))?NULL:&timeout)) > 0) {
            while ((size > 0) && ((length = esocketrecv(stream, pointer, size)) > 0)) {
                size -= length;
                pointer += length;
            }
            timeout.tv_sec = (int)sec;
            timeout.tv_usec = (int)usec;
        } else
            break;
    }
    if (size)
        result = EE_ERROR;
    return result;
}

void cclient::unload (void) {
    if (infos.stream != -1) {
        shutdown(infos.stream, SHUT_RDWR);
        esocketclose(infos.stream);
    }
#ifdef _WIN32
    WSACleanup();
#endif
    this->init();
}
