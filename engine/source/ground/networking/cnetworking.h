/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CNETWORK_H
#define EE_CNETWORK_H
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif
#define _WIN32_WINNT 0x0501
#include <unistd.h>
#include <stdarg.h>
#include <limits.h>
#include <limits.h>
#include <pthread.h>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <io.h>
#ifndef SHUT_RDWR
#define SHUT_RDWR 2
#endif
#ifndef EAGAIN
#define EAGAIN WSAEWOULDBLOCK
#endif
#define esocketclose(des)\
    do{\
        closesocket(des);\
        des=-1;\
    }while(0)
#define esocketrecv(des,ptr,siz)\
    recv(des, ptr, siz, 0)
#define esocketsend(des,hea,dat,siz)\
    ((send(des, hea, EE_HEADER_SIZE, 0) > 0) && (send(des, dat, siz, 0) > 0))
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/fcntl.h>
#include <netdb.h>
#include <errno.h>
#define esocketclose(des)\
    do{\
        close(des);\
        des=-1;\
    }while(0)
#define esocketrecv(des,ptr,siz)\
    read(des, ptr, siz)
#define esocketsend(des,hea,dat,siz)\
    ((write(des, hea, EE_HEADER_SIZE) > 0) && (write(des, dat, siz) > 0))
#endif
#include "cendian.h"
#include "carray.h"
#define EE_HEADER_SIZE 4+sizeof(size_t)
#define EE_BUFFER_SIZE 4
#define EE_CONTENT_SIZE 1024
#define EE_HEADER {'A','2','P','N'}
enum enetworkaction {
    ENETWORKACTION_HEADER,
    ENETWORKACTION_BODY
};
typedef struct ssocket {
    int stream;
/* private */
    size_t length;
    enetworkaction action;
/* private */
} ssocket;
#endif
