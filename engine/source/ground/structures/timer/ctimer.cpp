/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "ctimer.h"
int ctimer::add (const char *label) {
	stime *current = NULL;
	struct timeval btimer;
    int result = EE_OK;
	if ((current = (stime *) emalloc(sizeof(stime)))) {
		gettimeofday(&btimer, NULL);
		current->usecs = ((btimer.tv_sec*EE_USECS)+btimer.tv_usec)-kvalue;
        if (kvalue == 0) {
            kvalue = current->usecs;
            current->usecs = 0;
        }
		result = timers.add(current, label);
	} else
        ekill("out of memory");
	return result;
}

int ctimer::del (const char *label) {
    stime *current = NULL;
    int result = EE_OK;
    if ((current = timers.get(label))) {
        efree(current);
        timers.del();
    } else
        result = EE_ERROR;
	return result;
}

int ctimer::set (const char *label) {
    stime *current = NULL;
    struct timeval btimer;
    int result = EE_OK;
    if ((current = timers.get(label))) {
        gettimeofday(&btimer, NULL);
		current->usecs = ((btimer.tv_sec*EE_USECS)+btimer.tv_usec)-kvalue;
    } else
        result = EE_ERROR;
	return result;
}

stime ctimer::get (const char *label) {
    stime *current = NULL, result;
    struct timeval btimer;
    result.secs = result.usecs = 0;
    if ((current = timers.get(label))) {
        gettimeofday(&btimer, NULL);
        result.usecs = (((btimer.tv_sec*EE_USECS)+btimer.tv_usec)-kvalue)-current->usecs;
        result.secs = (result.usecs/EE_USECS);
    }
	return result;    
}

void ctimer::unload (void) {
    destroy(&timers, efree, stime);
    this->init();
}
