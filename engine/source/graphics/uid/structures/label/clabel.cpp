/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "clabel.h"
elabel& operator++ (elabel& value) { return value = (value==ELABEL_NULL)?ELABEL_BACKGROUND:elabel(value+1); }
cfonts *clabel::replacement = NULL;
void clabel::init (void) {
    int index;
    ccomponent::init(); /* father's init */
    characters.init();
    for (index = 0; index < ELABEL_NULL; index++)
        interface[index] = NULL;
    current = ELABEL_NULL;
    fonts = NULL;
    viewport[EPOSITION_POSITIONX] = 0;
    viewport[EPOSITION_POSITIONY] = 0;
    size = EE_LABEL_SIZE;
    interline = EE_LABEL_INTERLINE;
    paragraph = EE_LABEL_PARAGRAPH;
    selected = false;
    visible = true;
    background = EE_LABEL_BACKGROUND;
    drop(EPOSITION_POSITIONX, 0);
    drop(EPOSITION_POSITIONY, 0);
    drop(EPOSITION_WIDTH, EE_LABEL_W);
    drop(EPOSITION_HEIGHT, EE_LABEL_H);
}

int clabel::initialize (ctheme *theme) {
    int index, result = EE_OK;
    ethelayout component = ETHELAYOUT_NULL;
    for (index = 0; index < ELABEL_NULL; index++) {
        switch (index) {
            case ELABEL_CORNER_NW: component = ETHELAYOUT_COMPONENT_CORNER_NW; break;
            case ELABEL_CORNER_NE: component = ETHELAYOUT_COMPONENT_CORNER_NE; break;
            case ELABEL_CORNER_SW: component = ETHELAYOUT_COMPONENT_CORNER_SW; break;
            case ELABEL_CORNER_SE: component = ETHELAYOUT_COMPONENT_CORNER_SE; break;
            case ELABEL_BORDER_N: component = ETHELAYOUT_COMPONENT_BORDER_N; break;
            case ELABEL_BORDER_S: component = ETHELAYOUT_COMPONENT_BORDER_S; break;
            case ELABEL_BORDER_W: component = ETHELAYOUT_COMPONENT_BORDER_W; break;
            case ELABEL_BORDER_E: component = ETHELAYOUT_COMPONENT_BORDER_E; break;
            case ELABEL_BACKGROUND: component = ETHELAYOUT_COMPONENT_BACKGROUND; break;
        }
        interface[index] = theme->get(component);
    }
    if (!(fonts = theme->get()))
        fonts = replacement;
    return result;
}

void clabel::content (const char *value) {
    scharacter *character;
    size_t length = estrlen(value), index;
    destroy(&characters, efree, scharacter);
    for (index = 0; index < length; index++) {
        if ((character = (scharacter *) emalloc(sizeof(scharacter)))) {
            character->character = value[index];
            if (fonts)
                character->font = fonts->get("default");
            else
                character->font = NULL;
            characters.add(character);
        } else
            ekill("out of memory");
    }
    analyzing();
    calculate();
}

char *clabel::content (void) {
    scharacter *character;
    char *result = NULL;
    size_t length = characters.records(), index = 0;
    if (length > 0) {
        if ((result = (char *) emalloc(length+1))) {
            foreach(character, &characters)
                result[index++] = character->character;
            result[index] = '\0';
        } else
            ekill("out of memory");
    }
    return result;
}

void clabel::rebuild (elabel component) {
    int final[EPOSITION_NULL];
    if (interface[component]) {
        restore();
        final[EPOSITION_POSITIONX] = destination(EPOSITION_POSITIONX);
        final[EPOSITION_POSITIONY] = destination(EPOSITION_POSITIONY);
        final[EPOSITION_WIDTH] = interface[component]->source(EPOSITION_WIDTH);
        final[EPOSITION_HEIGHT] = interface[component]->source(EPOSITION_HEIGHT);
        switch (component) {
            case ELABEL_CORNER_NW:
                break;
            case ELABEL_CORNER_NE:
                final[EPOSITION_POSITIONX] = (destination(EPOSITION_POSITIONX)+destination(EPOSITION_WIDTH)-safesrc(interface[ELABEL_CORNER_NE], EPOSITION_WIDTH));
                break;
            case ELABEL_CORNER_SW:
                final[EPOSITION_POSITIONY] = (destination(EPOSITION_POSITIONY)+destination(EPOSITION_HEIGHT)-safesrc(interface[ELABEL_CORNER_SW], EPOSITION_HEIGHT));
                break;
            case ELABEL_CORNER_SE:
                final[EPOSITION_POSITIONX] = (destination(EPOSITION_POSITIONX)+destination(EPOSITION_WIDTH)-safesrc(interface[ELABEL_CORNER_SE], EPOSITION_WIDTH));
                final[EPOSITION_POSITIONY] = (destination(EPOSITION_POSITIONY)+destination(EPOSITION_HEIGHT)-safesrc(interface[ELABEL_CORNER_SE], EPOSITION_HEIGHT));
                break;
            case ELABEL_BORDER_N:
                final[EPOSITION_POSITIONX] = (destination(EPOSITION_POSITIONX)+safesrc(interface[ELABEL_CORNER_NW], EPOSITION_WIDTH));
                final[EPOSITION_WIDTH] = EE_MAX(0, (destination(EPOSITION_WIDTH)-safesrc(interface[ELABEL_CORNER_NW], EPOSITION_WIDTH)-safesrc(interface[ELABEL_CORNER_NE], EPOSITION_WIDTH)));
                break;
            case ELABEL_BORDER_S:
                final[EPOSITION_POSITIONX] = (destination(EPOSITION_POSITIONX)+safesrc(interface[ELABEL_CORNER_SW], EPOSITION_WIDTH));
                final[EPOSITION_POSITIONY] =  (destination(EPOSITION_POSITIONY)+destination(EPOSITION_HEIGHT)-safesrc(interface[ELABEL_CORNER_SW], EPOSITION_HEIGHT));
                final[EPOSITION_WIDTH] = EE_MAX(0, (destination(EPOSITION_WIDTH)-safesrc(interface[ELABEL_CORNER_SW], EPOSITION_WIDTH)-safesrc(interface[ELABEL_CORNER_SE], EPOSITION_WIDTH)));
                break;
            case ELABEL_BORDER_W:
                final[EPOSITION_POSITIONY] = (destination(EPOSITION_POSITIONY)+safesrc(interface[ELABEL_CORNER_NW], EPOSITION_HEIGHT));
                final[EPOSITION_HEIGHT] = EE_MAX(0, (destination(EPOSITION_HEIGHT)-safesrc(interface[ELABEL_CORNER_NW], EPOSITION_HEIGHT)-safesrc(interface[ELABEL_CORNER_NE], EPOSITION_HEIGHT)));
                break;
            case ELABEL_BORDER_E:
                final[EPOSITION_POSITIONX] = (destination(EPOSITION_POSITIONX)+destination(EPOSITION_WIDTH)-safesrc(interface[ELABEL_CORNER_SE], EPOSITION_WIDTH));
                final[EPOSITION_POSITIONY] = (destination(EPOSITION_POSITIONY)+safesrc(interface[ELABEL_CORNER_NW], EPOSITION_HEIGHT));
                final[EPOSITION_HEIGHT] = EE_MAX(0, (destination(EPOSITION_HEIGHT)-safesrc(interface[ELABEL_CORNER_NW], EPOSITION_HEIGHT)-safesrc(interface[ELABEL_CORNER_NE], EPOSITION_HEIGHT)));
                break;
            case ELABEL_BACKGROUND:
                final[EPOSITION_WIDTH] = destination(EPOSITION_WIDTH);
                final[EPOSITION_HEIGHT] = destination(EPOSITION_HEIGHT);
            default:
                break;  
        }
        interface[component]->drop(EPOSITION_POSITIONX, final[EPOSITION_POSITIONX]);
        interface[component]->drop(EPOSITION_POSITIONY, final[EPOSITION_POSITIONY]);
        interface[component]->drop(EPOSITION_WIDTH, final[EPOSITION_WIDTH]);
        interface[component]->drop(EPOSITION_HEIGHT, final[EPOSITION_HEIGHT]);        
    }
}

void clabel::analyzing (void) {
    /* do nothing */
}

void clabel::calculate (void) {
    sfont *fontset;
    scharacter *character;
    int position[EPOSITION_NULL], viewport[EPOSITION_NULL];
    overflow[EPOSITION_POSITIONX] = overflow[EPOSITION_POSITIONY] = overflow[EPOSITION_WIDTH] = overflow[EPOSITION_HEIGHT] = 0;
    foreach(character, &characters) {
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
    if (overflow[EPOSITION_POSITIONX] > overflow[EPOSITION_WIDTH])
        overflow[EPOSITION_WIDTH] = overflow[EPOSITION_POSITIONX];
    overflow[EPOSITION_HEIGHT] = (int)overflow[EPOSITION_POSITIONY]+interline;
    crop(EPOSITION_WIDTH, overflow[EPOSITION_WIDTH]);
    crop(EPOSITION_HEIGHT, overflow[EPOSITION_HEIGHT]);
}

GLuint clabel::get (void) {
    GLuint result = EE_NULLTEXTURE;
    sfont *fontset;
    scharacter *character = characters.get();
    int position[EPOSITION_NULL], viewport[EPOSITION_NULL];
    bool completed = false, jump = true, safeing = false;
    if (visible) {
        if (current == ELABEL_NULL) {
            if (!character)
                current = (background)?ELABEL_BACKGROUND:ELABEL_NULL;
        } else
            current++;
        if ((current != ELABEL_NULL) && (interface[current])) {
            rebuild((elabel)current);
            singletexture(result, interface[current]);
            coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_POSITIONX] = interface[current]->destination(EPOSITION_POSITIONX, EPOSITIONSUBJECT_FRAME);
            coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_POSITIONY] = interface[current]->destination(EPOSITION_POSITIONY, EPOSITIONSUBJECT_FRAME);
            coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_WIDTH] = interface[current]->destination(EPOSITION_WIDTH, EPOSITIONSUBJECT_FRAME);
            coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_HEIGHT] = interface[current]->destination(EPOSITION_HEIGHT, EPOSITIONSUBJECT_FRAME);
            coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_POSITIONX] = interface[current]->source(EPOSITION_POSITIONX, EPOSITIONSUBJECT_FRAME);
            coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_POSITIONY] = interface[current]->source(EPOSITION_POSITIONY, EPOSITIONSUBJECT_FRAME);
            coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_WIDTH] = interface[current]->source(EPOSITION_WIDTH, EPOSITIONSUBJECT_FRAME);
            coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_HEIGHT] = interface[current]->source(EPOSITION_HEIGHT, EPOSITIONSUBJECT_FRAME);
            flipped[EFLIPDIRECTION_HORIZONTAL] = interface[current]->flipped[EFLIPDIRECTION_HORIZONTAL];
            flipped[EFLIPDIRECTION_VERTICAL] = interface[current]->flipped[EFLIPDIRECTION_VERTICAL];
            channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_R] = mastercolor(interface[current]->gloss(ECOLOR_R, EPOSITIONSUBJECT_FRAME), channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_R]);
            channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_G] = mastercolor(interface[current]->gloss(ECOLOR_G, EPOSITIONSUBJECT_FRAME), channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_G]);
            channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_B] = mastercolor(interface[current]->gloss(ECOLOR_B, EPOSITIONSUBJECT_FRAME), channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_B]);
            channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_A] = mastercolor(interface[current]->gloss(ECOLOR_A, EPOSITIONSUBJECT_FRAME), channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_A]);
        } else {
            /* drawing contained components */
            if (!character) {
                overflow[EPOSITION_POSITIONX] = destination(EPOSITION_POSITIONX)+paragraph;
                overflow[EPOSITION_POSITIONY] = destination(EPOSITION_POSITIONY)+2;
            }
            character = characters.nextvalue();
            while (character) {
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
                            overflow[EPOSITION_POSITIONX] = destination(EPOSITION_POSITIONX)+paragraph;
                            break;
                    }
                } else {
                    viewport[EPOSITION_POSITIONX] = destination(EPOSITION_POSITIONX)+paragraph+this->viewport[EPOSITION_POSITIONX];
                    viewport[EPOSITION_POSITIONY] = destination(EPOSITION_POSITIONY)+2+this->viewport[EPOSITION_POSITIONY];
                    viewport[EPOSITION_WIDTH] = destination(EPOSITION_WIDTH)-safesrc(interface[ELABEL_BORDER_E], EPOSITION_WIDTH);
                    viewport[EPOSITION_HEIGHT] = destination(EPOSITION_HEIGHT);
                    position[EPOSITION_POSITIONX] = overflow[EPOSITION_POSITIONX]+paragraph;
                    position[EPOSITION_POSITIONY] = overflow[EPOSITION_POSITIONY]+2;
                    position[EPOSITION_WIDTH] = fontset->positions[(character->character-33)][EPOSITION_WIDTH];
                    position[EPOSITION_HEIGHT] = fontset->positions[(character->character-33)][EPOSITION_HEIGHT];
                    if ((position[EPOSITION_POSITIONX] >= viewport[EPOSITION_POSITIONX]) && ((position[EPOSITION_POSITIONX]+position[EPOSITION_WIDTH]) <= (viewport[EPOSITION_POSITIONX]+viewport[EPOSITION_WIDTH]))) {
                        if ((position[EPOSITION_POSITIONY] >= viewport[EPOSITION_POSITIONY]) && ((position[EPOSITION_POSITIONY]+position[EPOSITION_HEIGHT]) <= (viewport[EPOSITION_POSITIONY]+viewport[EPOSITION_HEIGHT]))) {
                            singletexture(result, fontset->layer);
                            coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_POSITIONX] = overflow[EPOSITION_POSITIONX]-this->viewport[EPOSITION_POSITIONX];
                            coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_POSITIONY] = overflow[EPOSITION_POSITIONY]-this->viewport[EPOSITION_POSITIONY];
                            coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_WIDTH] = fontset->positions[(character->character-33)][EPOSITION_WIDTH];
                            coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_HEIGHT] = fontset->positions[(character->character-33)][EPOSITION_HEIGHT];
                            coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_POSITIONX] = fontset->positions[(character->character-33)][EPOSITION_POSITIONX];
                            coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_POSITIONY] = fontset->positions[(character->character-33)][EPOSITION_POSITIONY];
                            coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_WIDTH] = fontset->positions[(character->character-33)][EPOSITION_WIDTH];
                            coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_HEIGHT] = fontset->positions[(character->character-33)][EPOSITION_HEIGHT];
                            flipped[EFLIPDIRECTION_HORIZONTAL] = fontset->layer->flipped[EFLIPDIRECTION_HORIZONTAL];
                            flipped[EFLIPDIRECTION_VERTICAL] = fontset->layer->flipped[EFLIPDIRECTION_VERTICAL];
                            channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_R] = mastercolor(fontset->channels.color[ECOLOR_R], channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_R]);
                            channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_G] = mastercolor(fontset->channels.color[ECOLOR_G], channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_G]);
                            channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_B] = mastercolor(fontset->channels.color[ECOLOR_B], channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_B]);
                            channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_A] = mastercolor(fontset->channels.color[ECOLOR_A], channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_A]);
                            completed = true;
                        }
                        overflow[EPOSITION_POSITIONX] += fontset->positions[(character->character-33)][EPOSITION_WIDTH];
                    } else {
                        overflow[EPOSITION_POSITIONY] += interline;
                        overflow[EPOSITION_POSITIONX] = destination(EPOSITION_POSITIONX)+paragraph+this->viewport[EPOSITION_POSITIONX];
                        jump = safeing;
                    }
                    if (completed)
                        break;
                }
                if (jump) {
                    character = characters.nextvalue();
                    safeing = false;
                } else
                    safeing = true;
                jump = true;
            }
        }
    }
    return result;
}

void clabel::restore (void) {
    int index;
    for (index = 0; index < ELABEL_NULL; index++)
        if (interface[index])
            interface[index]->restore();
    channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_R] = channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_G] = channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_B] = channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_A] = 1.0f;
    channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_R] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_G] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_B] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_A] = 1.0f;
    rotation = 0.0f;
}

void clabel::forward (skeyboard *keyboard, smouse *mouse, unsigned int positionx, unsigned int positiony) {
    /* do nothing */
}

void clabel::unload (void) {
    destroy(&characters, efree, scharacter);
    this->init();
}
