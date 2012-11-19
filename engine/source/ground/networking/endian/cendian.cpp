/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "cendian.h"
int cendian::check (void) {
    unsigned int value = 0xAABBCCDD;
	return (*((unsigned char *)&value)==0xAA)?EE_BIG_ENDIAN:EE_LITTLE_ENDIAN;
}

void cendian::swap (void *data, size_t length) {
    char *pointer = (char *)data, byte;
    size_t tail, head;
    for (head = 0, tail = length-1; tail > head; head++, tail--) {
        byte = pointer[head];
        pointer[head] = pointer[tail];
        pointer[tail] = byte;
    }
}
