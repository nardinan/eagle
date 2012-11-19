/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "clog.h"
void clog::log (FILE *stream, const char *function, const char *format, ...) {
    va_list arguments;
    time_t instant = time(NULL);
    struct tm *local = localtime(&instant);
    char timebuffer[EE_LABEL_SIZE], messagebuffer[EE_LABEL_SIZE];
    strftime(timebuffer, EE_LABEL_SIZE, "%b %a %d @ %H:%M", local);
    va_start(arguments, format);
    {
        vsnprintf(messagebuffer, EE_LABEL_SIZE, format, arguments);
        fprintf(stream, "[%s] - (%s): %s\n", function, timebuffer, messagebuffer);
    }
    va_end(arguments);
    fflush(stream);
}
