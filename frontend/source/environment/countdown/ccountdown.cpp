/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Filippo Paolucci
 * EMAIL        : fantic86@alice.it
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "ccountdown.h"
int ccountdown::check(const char* label, int countdown_ms) {
    stime time = this->get(label);
    int usecs, secs, result = EE_OK;
    usecs = (abs(countdown_ms)%1000)*1000;
    secs = (int)(abs(countdown_ms))/1000.0f;
    if ((time.secs >= secs) && (time.usecs >= usecs)) {
        if (EE_CCOUNTDOWN_PRINT)
            elog("ms: countdown finished");
        this->set(label);
    } else
        result = EE_ERROR;
    if ((time.usecs == 0) && (time.secs == 0)) {
        if (EE_CCOUNTDOWN_PRINT)
            elog("label: time added to countdown");
        result = EE_ERROR;
    }
    return result;
}

