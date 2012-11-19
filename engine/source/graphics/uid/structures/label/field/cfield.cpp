/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "cfield.h"
void cfield::init (void) {
    clabel::init(); /* father's init */
    size = EE_FIELD_SIZE;
    background = EE_FIELD_BACKGROUND;
    selected = false;
    multiline = EE_FIELD_MULTILINE;
    editable = EE_FIELD_EDITABLE;
}

void cfield::forward (skeyboard *keyboard, smouse *mouse, unsigned int positionx, unsigned int positiony) {
    int bskey, rckey, absolute[EPOSITION_NULL];
    scharacter *character;
#ifndef _WIN32
    bskey = 127;
    rckey = 13;
#else
    bskey = 8;
    rckey = 13;
#endif
    if (mouse) {
        absolute[EPOSITION_POSITIONX] = mouse->positionx;
        absolute[EPOSITION_POSITIONY] = mouse->positiony;
        mouse->positionx = positionx;
        mouse->positiony = positiony;
        inrange(mouse, destination(EPOSITION_POSITIONX), destination(EPOSITION_POSITIONY), destination(EPOSITION_WIDTH), destination(EPOSITION_HEIGHT), selected);
        if ((mouse->action == EACTION_RELEASED) && (mouse->key == EMOUSE_LEFT))
            this->selected = selected;
        mouse->positionx = absolute[EPOSITION_POSITIONX];
        mouse->positiony = absolute[EPOSITION_POSITIONY];
    }
    if ((selected) && (keyboard) && (editable)) {
        if (keyboard->action == EACTION_PRESSED) {
            if ((keyboard->key >= (int)32) && (keyboard->key <= (int)126) && ((characters.records()+1) <= size)) {
                if ((character = (scharacter *) emalloc(sizeof(scharacter)))) {
                    character->character = keyboard->key;
                    character->font = NULL;
                    characters.add(character);
                } else
                    ekill("out of memory");
            } else if ((keyboard->key == bskey) && (characters.records() > 0)) {
                if ((character = characters.get(characters.records()-1))) {
                    efree(character);
                    characters.del();
                }
            } else if ((keyboard->key == rckey) && (multiline)) {
                if ((character = (scharacter *) emalloc(sizeof(scharacter)))) {
                    character->character = '\n';
                    character->font = NULL;
                    characters.add(character);
                } else
                    ekill("out of memory");
            }
            analyzing();
            calculate();
        }
    }
}
