/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "cchannels.h"
void cchannels::init (void) {
    channels.init();
}

int cchannels::listening (cxml &parameters) {
    sxmlclass *xmlsingleton[2] = {NULL, NULL};
    schannel *channel;
    char *value;
    int result = EE_OK;
    /*
     <channels>
        <channel ID=identify volume=255 exclusive=false />
        [...]
     </channels>
     */
    if ((xmlsingleton[0] = parameters.get("ss", "engine", "channels"))) {
        foreach(xmlsingleton[1], &xmlsingleton[0]->subclass) {
            if (estrcmp(xmlsingleton[1]->label, "channel") == 0) {
                if ((channel = (schannel *) emalloc(sizeof(schannel)))) {
                    channel->volume = ((value = xmlsingleton[1]->keys.get("volume"))?EE_MAX(eatoi(value), 255):0);
                    channel->exclusive = ((value = xmlsingleton[1]->keys.get("exclusive"))?(estrcmp(value,"true")==0):false);
                    channel->samples.init();
                    result = channels.add(channel, xmlsingleton[1]->keys.get("ID"));
                } else
                    ekill("out of memory");
            }
        }
    }
    return result;
}

int cchannels::add (bool exclusive, unsigned int volume, const char *label) {
    schannel *channel;
    int result = EE_OK;
    if ((channel = (schannel *) emalloc(sizeof(schannel)))) {
        channel->volume = EE_MAX(volume, 255);
        channel->exclusive = exclusive;
        channel->samples.init();
        result = channels.add(channel, label);
    } else
        ekill("out of memory");
    return result;
}

int cchannels::set (const char *label) {
    int result = EE_OK;
    if (!(current = channels.get(label)))
        result = EE_ERROR;
    return result;
}

int cchannels::append (csample *sample, const char *label) {
    int result = EE_OK;
    if (current)
        result = current->samples.add(sample, label);
    else
        result = EE_ERROR;
    return result;
}

int cchannels::play (const char *label) {
    csample *sample;
    int result = EE_OK;
    if (current) {
        if ((sample = current->samples.get(label))) {
            if (((current->exclusive) && (!playing())) || (!current->exclusive))
                sample->play();
        } else
            result = EE_ERROR;
    } else
        result = EE_ERROR;
    return result;
}

int cchannels::stop (const char *label) {
    csample *sample;
    int result = EE_OK;
    if (current) {
        if ((sample = current->samples.get(label)))
            sample->stop();
        else
            result = EE_ERROR;
    } else
        result = EE_ERROR;
    return result;
}

int cchannels::stop (void) {
    csample *sample;
    int result = EE_OK;
    if (current) {
        foreach(sample, &current->samples)
            if (sample->playing())
                sample->stop();
    } else
        result = EE_ERROR;
    return result;
}
                                 
int cchannels::pause (const char *label) {
    csample *sample;
    int result = EE_OK;
    if (current) {
        if ((sample = current->samples.get(label)))
            sample->pause();
        else
            result = EE_ERROR;
    } else
        result = EE_ERROR;
    return result;
}

bool cchannels::playing (const char *label) {
    csample *sample;
    bool result = false;
    if (current) {
        if ((sample = current->samples.get(label)))
            result = sample->playing();
    }
    return result;
}

bool cchannels::playing (void) {
    csample *sample;
    bool result = false;
    if (current) {
        foreach(sample, &current->samples)
            if ((result = sample->playing()))
                break;
    }
    return result;
}

void cchannels::volume (unsigned int value) {
    csample *sample;
    if (current) {
        current->volume = value;
        foreach(sample, &current->samples)
            sample->volume(value);
    }
}

void cchannels::unload (schannel *singleton) {
    singleton->samples.unload();
    efree(singleton);
}

void cchannels::unload (void) {
    destroy(&channels, this->unload, schannel);
}

