/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "cenvironment.h"
void cenvironment::init (void) {
    environment = current = NULL;
    elements = 0;
}

int cenvironment::add (ccontext *context, unsigned int layer, const char *label) {
    scontext *block = NULL;
    int result = EE_OK;
    if ((block = (scontext *) emalloc(sizeof(scontext)))) {
        memset(block->label, '\0', (sizeof(char)*EE_LABEL_SIZE));
        block->context = context;
        if (label)
            strncpy(block->label, label, (EE_LABEL_SIZE-1));
        block->layer = layer;
        block->next = block->back = NULL;
        current = environment;
        while (current)
            if (current->layer < block->layer) {
                if (current->next)
                    current = current->next;
                else
                    break;
            } else
                break;
        if (current) {
            if (current->layer >= block->layer) {
                block->next = current;
                if ((block->back = current->back))
                    current->back->next = block;
                else
                    environment = block;
                current->back = block;
            } else {
                current->next = block;
                block->back = current;
            }
        } else
            environment = block;
        elements++;
    } else
        ekill("out of memory");
    return result;
}

void cenvironment::del (void) {
    if (current) {
        if (current->next)
            current->next->back = current->back;
        if (current->back)
            current->back->next = current->next;
        else
            environment = current->next;
        efree(current);
        current = NULL;
        elements--;
    }
}

ccontext *cenvironment::get (const char *label) {
    current = environment;
    while (current) {
        if (estrcmp(current->label, label) == 0)
            break;
        current = current->next;
    }
    return (current)?current->context:NULL;
}

ccontext *cenvironment::get (ccontext *context) {
    current = environment;
    while (current) {
        if (current->context == context)
            break;
        current = current->next;
    }
    return (current)?current->context:NULL;
}

void cenvironment::next (void) {
    if (!current)
        current = environment;
    else
        current = current->next;
}

ccontext *cenvironment::nextvalue (void) {
    this->next();
    return (current)?current->context:NULL;
}

char *cenvironment::nextkey (void) {
    this->next();
    return (current)?current->label:NULL;
}

void cenvironment::unload (void) {
    while (environment) {
        current = environment;
        environment = environment->next;
        efree(current);
    }
    this->init();
} 
