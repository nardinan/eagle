/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "clist.h"
void clist::init (void) {
    clabel::init(); /* father's init */
    memset(choosed, '\0', (sizeof(char)*EE_LABEL_SIZE));
    handler = NULL;
    background = EE_LIST_BACKGROUND;
    selected = false;
}

void clist::handleizer (const char *identifier, flisthandler *handler) {
    strncpy(this->identifier, identifier, EE_LABEL_SIZE);
    this->identifier[EE_MIN(estrlen(identifier), (EE_LABEL_SIZE-1))] = '\0';
    this->handler = handler;
}

void clist::forward (skeyboard *keyboard, smouse *mouse, unsigned int positionx, unsigned int positiony) {
    bool present;
    int absolute[EPOSITION_NULL], result, index = 0;
    if (mouse) {
        absolute[EPOSITION_POSITIONX] = mouse->positionx;
        absolute[EPOSITION_POSITIONY] = mouse->positiony;
        mouse->positionx = positionx;
        mouse->positiony = positiony;
        inrange(mouse, destination(EPOSITION_POSITIONX), destination(EPOSITION_POSITIONY), destination(EPOSITION_WIDTH), destination(EPOSITION_HEIGHT), present);
        if (present) {
            if ((mouse->action == EACTION_RELEASED) && (mouse->key == EMOUSE_LEFT)) {
                if ((result = search(mouse->positionx, mouse->positiony)) >= 0) {
                    for (;result < characters.records(); result++) {
                        if (characters.get(result)->character == '\n')
                            break;
                        choosed[index++] = characters.get(result)->character;
                    }
                    choosed[index] = '\0';
                    if (handler)
                        handler(this, identifier, choosed);
                }
            }
        }
        mouse->positionx = absolute[EPOSITION_POSITIONX];
        mouse->positiony = absolute[EPOSITION_POSITIONY];
    }
}

int clist::search (unsigned int positionx, unsigned int positiony) {
    sfont *fontset;
    scharacter *character;
    int position[EPOSITION_NULL], viewport[EPOSITION_NULL], overflow[EPOSITION_NULL], choose = 0, index;
    bool founded = false;
    overflow[EPOSITION_POSITIONX] = destination(EPOSITION_POSITIONX)+paragraph;
    overflow[EPOSITION_POSITIONY] = destination(EPOSITION_POSITIONY)+2;
    overflow[EPOSITION_WIDTH] = overflow[EPOSITION_HEIGHT] = 0;
    if ((positiony >= overflow[EPOSITION_POSITIONY]-this->viewport[EPOSITION_POSITIONY]) && (positiony <= overflow[EPOSITION_POSITIONY]-this->viewport[EPOSITION_POSITIONY]+interline))
        founded = true;
    for (index = 0; index < characters.records() && (!founded); index++) {
        if ((character = characters.get(index))) {
            if ((positiony >= overflow[EPOSITION_POSITIONY]-this->viewport[EPOSITION_POSITIONY]) && (positiony <= overflow[EPOSITION_POSITIONY]-this->viewport[EPOSITION_POSITIONY]+interline))
                founded = true;
            if (!founded) {
                if (fonts)
                    fontset = ((character->font)?character->font:fonts->get("default"));
                else
                    fontset = ((character->font)?character->font:replacement->get("default"));
                if ((character->character == ' ') || (character->character == '\n')) {
                    switch (character->character) {
                        case ' ': 
                            overflow[EPOSITION_POSITIONX] += fontset->space;
                            break;
                        case '\n':
                            overflow[EPOSITION_POSITIONY] += interline;
                            if (overflow[EPOSITION_POSITIONX] > overflow[EPOSITION_WIDTH])
                                overflow[EPOSITION_WIDTH] = overflow[EPOSITION_POSITIONX];
                            overflow[EPOSITION_POSITIONX] = destination(EPOSITION_POSITIONX)+paragraph;
                            choose = (index+1);
                            break;
                    }
                } else {
                    viewport[EPOSITION_POSITIONX] = destination(EPOSITION_POSITIONX)+paragraph+this->viewport[EPOSITION_POSITIONX];
                    viewport[EPOSITION_WIDTH] = destination(EPOSITION_WIDTH)-safesrc(interface[ELABEL_BORDER_E], EPOSITION_WIDTH);
                    position[EPOSITION_POSITIONX] = overflow[EPOSITION_POSITIONX]+paragraph;
                    position[EPOSITION_WIDTH] = fontset->positions[(character->character-33)][EPOSITION_WIDTH];
                    if ((position[EPOSITION_POSITIONX] >= viewport[EPOSITION_POSITIONX]) && ((position[EPOSITION_POSITIONX]+position[EPOSITION_WIDTH]) <= (viewport[EPOSITION_POSITIONX]+viewport[EPOSITION_WIDTH])))
                        overflow[EPOSITION_POSITIONX] += fontset->positions[(character->character-33)][EPOSITION_WIDTH];
                    else {
                        overflow[EPOSITION_POSITIONY] += interline;
                        if (overflow[EPOSITION_POSITIONX] > overflow[EPOSITION_WIDTH])
                            overflow[EPOSITION_WIDTH] = overflow[EPOSITION_POSITIONX];
                        overflow[EPOSITION_POSITIONX] = destination(EPOSITION_POSITIONX)+paragraph;
                    }
                }
            }
        }
    }
    return (founded)?choose:(-1);
}
