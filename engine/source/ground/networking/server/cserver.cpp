/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "cserver.h"
int cserver::initialize (const char *address, const char *port, unsigned int queue) {
    struct addrinfo hints = {AI_PASSIVE, AF_INET, SOCK_STREAM}, *response = NULL;
	int result = EE_OK, value = 1;
#ifdef _WIN32
	WSAStartup(MAKEWORD(1,1), &datasocket);
#endif
	if (getaddrinfo(address, port, &hints, &response) == 0) {
		if ((stream = socket(response->ai_family, response->ai_socktype, response->ai_protocol)) > -1) {
			if (setsockopt(stream, SOL_SOCKET, SO_REUSEADDR, (char *)&value, sizeof(int)) != -1) {
#ifdef SO_NOSIGPIPE
                setsockopt(stream, SOL_SOCKET, SO_NOSIGPIPE, (char *)&value, sizeof(int));
#endif
				if (bind(stream, response->ai_addr, response->ai_addrlen) == 0) {
					if (listen(stream, queue) != 0) { 
                        freeaddrinfo(response); 
                        result = EE_ERROR;
                    }
				} else
                    result = EE_ERROR;
			} else
                result = EE_ERROR;
			if (result != EE_OK)
                esocketclose(stream);
		} else
            result = EE_ERROR;
		freeaddrinfo(response);
	}
	return result;
}

ssocket *cserver::search (unsigned int sec, unsigned int usec) {
    fd_set rdset;
	int client, flags;
	ssocket *result = NULL;
	struct timeval timeout = {(int)sec, (int)usec};
    unsigned long mode = 1;
	FD_ZERO(&rdset);
	FD_SET(stream, &rdset);
	if (select(stream+1, &rdset, NULL, NULL, (((timeout.tv_sec==0)&&(timeout.tv_usec==0))?NULL:&timeout)) > 0) {
		if ((client = accept(stream, NULL, NULL)) != -1) {
			if ((result = (ssocket *) emalloc(sizeof(ssocket)))) {
				result->stream = client;
				result->action = ENETWORKACTION_HEADER;
                result->length = 0;
#ifndef _WIN32
                mode = 0; /* warning avoid */
				if ((flags = fcntl(client, F_GETFL)) != -1)
                    fcntl(client, F_SETFL, flags|O_NONBLOCK);
#else
                ioctlsocket(client, FIONBIO, &mode);
#endif
			} else
                ekill("out of memory");
		}
    }
	return result;
}

int cserver::sending (ssocket *client, const char *format, ...) {
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
            introduction[index] = (shifted >> (8*(index-EE_BUFFER_SIZE))) & 0xff;
        result = esocketsend(client->stream, introduction, datablock, length);
    }
    va_end(args);
	return result;
}

int cserver::receiving (ssocket *client, char **datablock, unsigned int sec, unsigned int usec) {
    fd_set rdset;
	char *pointer, introduction[EE_HEADER_SIZE], parser[EE_BUFFER_SIZE] = EE_HEADER;
	struct timeval timeout = {(int)sec, (int)usec};
    int index, result = EE_OK;
    FD_ZERO(&rdset);
    FD_SET(client->stream, &rdset);
    if (select(client->stream+1, &rdset, NULL, NULL, (((timeout.tv_sec==0)&&(timeout.tv_usec==0))?NULL:&timeout)) > 0) {
        switch (client->action) {
            case ENETWORKACTION_HEADER:
                if ((result = receiving(client->stream, (char *)introduction, EE_HEADER_SIZE, 0, 1)) == EE_OK) {
                    pointer = (&introduction[EE_BUFFER_SIZE]);
                    for (index = 0; index < EE_BUFFER_SIZE; index++)
                        if (introduction[index] != parser[index])
                            break;
                    if (index == EE_BUFFER_SIZE) {
                        memcpy(&client->length, pointer, sizeof(size_t));
                        if (cendian::check() == EE_BIG_ENDIAN)
                            eswap(client->length);
                        client->action = ENETWORKACTION_BODY;
                    } /* skipping corrupted header */
                }
                break;
            case ENETWORKACTION_BODY:
                if ((*datablock = (char *) emalloc(client->length+1))) {
                    if (receiving(client->stream, *datablock, client->length, 0, 1) != EE_OK) {
                        efree(*datablock);
                        *datablock = NULL;
                        result = EE_ERROR;
                    }
                    client->action = ENETWORKACTION_HEADER;                            
                } else
                    ekill("out of memory");
                break;
        }
    }
	return result;
}

int cserver::receiving (int stream, char *datablock, size_t size, unsigned int sec, unsigned int usec) {
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

void cserver::unload (ssocket *client) {
    shutdown(client->stream, SHUT_RDWR);
    esocketclose(client->stream);
    efree(client);
}

void cserver::unload (void) {
    esocketclose(stream);
#ifdef _WIN32
    WSACleanup();
#endif
    this->init();
}
