/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "cwave.h"
void cwave::init (void) {
    csample::init(); /* father's init */
    size = 0;
    data = NULL;
}

int cwave::initialize (sdfssegment *datablock, bool loop) {
    unsigned char *pointer;
    int result = EE_OK;
    bool readed = false;
    pointer = datablock->datablock;
    schunkheader chunk;
    if (datablock->header.size > sizeof(swavheader)) {
        memcpy(&header, pointer, sizeof(swavheader));
        if (cendian::check() == EE_BIG_ENDIAN)
            eswap(header.size);
        if (check4(header.descriptor, "RIFF")) {
            if (check4(header.format, "WAVE")) {
                pointer += sizeof(swavheader);
                while (!readed) {
                    if (pointer < (datablock->datablock+datablock->header.size)) {
                        memcpy(&chunk, pointer, sizeof(schunkheader));
                        if (cendian::check() == EE_BIG_ENDIAN)
                            eswap(header.size);
                        if (check3(chunk.ID, "fmt")) {
                            memcpy(&informations, (pointer+sizeof(schunkheader)), sizeof(sfmtheader));
                            if (cendian::check() == EE_BIG_ENDIAN) {
                                eswap(informations.format);
                                eswap(informations.channels);
                                eswap(informations.sample);
                                eswap(informations.byte);
                                eswap(informations.align);
                                eswap(informations.bps);
                                eswap(informations.extra);
                            }
                        } else if (check4(chunk.ID, "data")) {
                            size = chunk.size;
                            if ((data = (unsigned char *) emalloc(chunk.size)))
                                memcpy(data, (pointer+sizeof(schunkheader)), chunk.size);
                            else
                                ekill("out of memory"); 
                            readed = true;
                        }
                        pointer += sizeof(schunkheader)+chunk.size;
                    } else
                        result = EE_ERROR;
                    if (!result)
                        break;
                }
                if (result)
                    if (!(result = sampleizer(loop)))
                        sample = EE_NULLSAMPLE;
            } else
                result = EE_ERROR;        
        } else
            result = EE_ERROR;
    } else
        result = EE_ERROR;
    return result;
}

void cwave::play (void) {
    alSourcePlay(sample);
}

void cwave::stop (void) {
    alSourceStop(sample);
    alSourceRewind(sample);
}

void cwave::pause (void) {
    alSourcePause(sample);
}

bool cwave::playing (void) {
    ALenum infos;
    alGetSourcei(sample, AL_SOURCE_STATE, &infos);
    return (infos==AL_PLAYING);
}

void cwave::volume (unsigned int value) {
    this->audio = value;
    if (sample != EE_NULLSAMPLE)
        alSourcef(sample, AL_GAIN, ((float)value/255.0f));
}

int cwave::sampleizer (bool loop) {
    int result = EE_OK;
    if (data) {
        if (sample != EE_NULLSAMPLE)
            alDeleteSources(1, &sample);
        if (buffer != EE_NULLSAMPLE)
            alDeleteBuffers(1, &buffer);
        alGenSources(1, &sample);
        alSourcef(sample, AL_PITCH, 1.0f);
        alSourcef(sample, AL_GAIN,((float)audio/255.0f));
        alSource3f(sample, AL_POSITION, 0.0f, 0.0f, 0.0f);
        alSource3f(sample, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
        alSourcei(sample, AL_LOOPING, loop);
        alSourcei(sample, AL_BUFFER, buffer);
        alGenBuffers(1, &buffer);
        alBufferData(buffer, cwave::format(informations.channels, informations.bps), data, size, informations.sample);
        alSourceQueueBuffers(sample, 1, &buffer);
    } else
        result = EE_ERROR;
    return result;
}

void cwave::unload (void) {
    if (data)
        efree(data);
    if (sample != EE_NULLSAMPLE)
        alDeleteSources(1, &sample);
    if (buffer != EE_NULLSAMPLE)
        alDeleteBuffers(1, &buffer);
    this->init();
}