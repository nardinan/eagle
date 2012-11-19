/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "cscroller.h"
escroller& operator++ (escroller& value) { return value = (value==ESCROLLER_NULL)?ESCROLLER_ARROW_UP:escroller(value+1); }
void cscroller::init (clabel *component) {
    int index;
    ccomponent::init(); /* father's init */
    for (index = 0; index < ESCROLLER_NULL; index++)
        interface[index] = NULL;
    value = component;
    current = ESCROLLER_NULL;
    components = false;
    increment = EE_SCROLLER_MOVEMENT;
    space = EE_SCROLLER_SPACE;
    selected = false;
    visible = true;
    background = EE_SCROLLER_BACKGROUND;
    drop(EPOSITION_HEIGHT, component->destination(EPOSITION_WIDTH));
}

int cscroller::initialize (ctheme *theme) {
    int index, result = EE_OK;
    ethelayout component = ETHELAYOUT_NULL;
    for (index = 0; index < ESCROLLER_NULL; index++) {
        switch (index) {
            case ESCROLLER_ARROW_UP: component = ETHELAYOUT_COMPONENT_SCROLLING_ARROW_UP; break;
            case ESCROLLER_ARROW_MIDDLE: component = ETHELAYOUT_COMPONENT_SCROLLING_MIDDLE; break;
            case ESCROLLER_ARROW_DOWN: component = ETHELAYOUT_COMPONENT_SCROLLING_ARROW_DOWN; break;
            case ESCROLLER_SCROLLER: component = ETHELAYOUT_COMPONENT_SCROLLING_SCROLLER; break;
        }
        interface[index] = theme->get(component);
    }
    return result;
}

void cscroller::rebuild (escroller component) {
    int final[EPOSITION_NULL];
    unsigned int vertical;
    if (interface[component]) {
        restore();
        drop(EPOSITION_POSITIONX, (value->destination(EPOSITION_POSITIONX)+value->destination(EPOSITION_WIDTH)+5));
        drop(EPOSITION_POSITIONY, value->destination(EPOSITION_POSITIONY));
        drop(EPOSITION_HEIGHT, value->destination(EPOSITION_HEIGHT));
        final[EPOSITION_POSITIONX] = destination(EPOSITION_POSITIONX);
        final[EPOSITION_POSITIONY] = destination(EPOSITION_POSITIONY);
        final[EPOSITION_WIDTH] = interface[component]->source(EPOSITION_WIDTH);
        final[EPOSITION_HEIGHT] = interface[component]->source(EPOSITION_HEIGHT);
        switch (component) {
            case ESCROLLER_ARROW_UP:
                break;
            case ESCROLLER_ARROW_MIDDLE:
                final[EPOSITION_POSITIONY] = (value->destination(EPOSITION_POSITIONY)+safesrc(interface[ESCROLLER_ARROW_UP], EPOSITION_HEIGHT));
                final[EPOSITION_HEIGHT] = EE_MAX(0, (destination(EPOSITION_HEIGHT)-safesrc(interface[ESCROLLER_ARROW_UP], EPOSITION_HEIGHT)-safesrc(interface[ESCROLLER_ARROW_DOWN], EPOSITION_HEIGHT)));
                break;
            case ESCROLLER_ARROW_DOWN:
                final[EPOSITION_POSITIONY] = (destination(EPOSITION_POSITIONY)+destination(EPOSITION_HEIGHT)-safesrc(interface[ESCROLLER_ARROW_DOWN], EPOSITION_HEIGHT));
                break;
            case ESCROLLER_SCROLLER:
                vertical = EE_MAX(0, (destination(EPOSITION_HEIGHT)-safesrc(interface[ESCROLLER_ARROW_UP], EPOSITION_HEIGHT)-safesrc(interface[ESCROLLER_ARROW_DOWN], EPOSITION_HEIGHT)-safesrc(interface[ESCROLLER_SCROLLER], EPOSITION_HEIGHT)));
                final[EPOSITION_POSITIONY] = (value->destination(EPOSITION_POSITIONY)+safesrc(interface[ESCROLLER_ARROW_UP], EPOSITION_HEIGHT))+EE_MIN(vertical, ((value->view(EPOSITION_POSITIONY)*vertical)/EE_MAX(1, ((int)value->height()-(int)value->destination(EPOSITION_HEIGHT)))));
                break;
            default:
                break;
        }
        interface[component]->drop(EPOSITION_POSITIONX, final[EPOSITION_POSITIONX]);
        interface[component]->drop(EPOSITION_POSITIONY, final[EPOSITION_POSITIONY]);
        interface[component]->drop(EPOSITION_WIDTH, final[EPOSITION_WIDTH]);
        interface[component]->drop(EPOSITION_HEIGHT, final[EPOSITION_HEIGHT]);  
    }
}

GLuint cscroller::get (void) {
    GLuint result = EE_NULLTEXTURE;
    if (visible) {
        if (current == ESCROLLER_NULL) {
            if (!components)
                current = (background)?ESCROLLER_ARROW_UP:ESCROLLER_NULL;
        } else
            current++;
        if ((current != ESCROLLER_NULL) && (interface[current])) {
            rebuild((escroller)current);
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
            if (!components)
                components = true;
            if ((result = value->get()) != EE_NULLTEXTURE) {
                coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_POSITIONX] = value->destination(EPOSITION_POSITIONX, EPOSITIONSUBJECT_FRAME);
                coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_POSITIONY] = value->destination(EPOSITION_POSITIONY, EPOSITIONSUBJECT_FRAME);
                coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_WIDTH] = value->destination(EPOSITION_WIDTH, EPOSITIONSUBJECT_FRAME);
                coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_HEIGHT] = value->destination(EPOSITION_HEIGHT, EPOSITIONSUBJECT_FRAME);
                coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_POSITIONX] = value->source(EPOSITION_POSITIONX, EPOSITIONSUBJECT_FRAME);
                coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_POSITIONY] = value->source(EPOSITION_POSITIONY, EPOSITIONSUBJECT_FRAME);
                coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_WIDTH] = value->source(EPOSITION_WIDTH, EPOSITIONSUBJECT_FRAME);
                coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_HEIGHT] = value->source(EPOSITION_HEIGHT, EPOSITIONSUBJECT_FRAME);
                flipped[EFLIPDIRECTION_HORIZONTAL] = value->flipped[EFLIPDIRECTION_HORIZONTAL];
                flipped[EFLIPDIRECTION_VERTICAL] = value->flipped[EFLIPDIRECTION_VERTICAL];
                channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_R] = mastercolor(value->gloss(ECOLOR_R, EPOSITIONSUBJECT_FRAME),channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_R]);
                channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_G] = mastercolor(value->gloss(ECOLOR_G, EPOSITIONSUBJECT_FRAME),channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_G]);
                channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_B] = mastercolor(value->gloss(ECOLOR_B, EPOSITIONSUBJECT_FRAME),channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_B]);
                channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_A] = mastercolor(value->gloss(ECOLOR_A, EPOSITIONSUBJECT_FRAME),channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_A]);
                
            } else
                components = false;
        }
    }
    return result;
}

void cscroller::restore (void) {
    int index;
    for (index = 0; index < ESCROLLER_NULL; index++)
        if (interface[index])
            interface[index]->restore();
    channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_R] = channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_G] = channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_B] = channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_A] = 1.0f;
    channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_R] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_G] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_B] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_A] = 1.0f;
    rotation = 0.0f;
}

void cscroller::forward (skeyboard *keyboard, smouse *mouse, unsigned int positionx, unsigned int positiony) {
    bool present;
    int absolute[EPOSITION_NULL], position[EPOSITION_NULL];
    unsigned int viewport;
    if (mouse) {
        absolute[EPOSITION_POSITIONX] = mouse->positionx;
        absolute[EPOSITION_POSITIONY] = mouse->positiony;
        mouse->positionx = positionx;
        mouse->positiony = positiony;
        if (mouse->action == EACTION_PRESSED) {
            position[EPOSITION_POSITIONX] = destination(EPOSITION_POSITIONX);
            position[EPOSITION_POSITIONY] = destination(EPOSITION_POSITIONY);
            position[EPOSITION_WIDTH] =  safesrc(interface[ESCROLLER_ARROW_UP], EPOSITION_WIDTH);
            position[EPOSITION_HEIGHT] = safesrc(interface[ESCROLLER_ARROW_UP], EPOSITION_HEIGHT);
            inrange(mouse, position[EPOSITION_POSITIONX], position[EPOSITION_POSITIONY], position[EPOSITION_WIDTH], position[EPOSITION_HEIGHT], present);
            if (present) {
                /* up arrow pressed */
                if ((viewport = value->view(EPOSITION_POSITIONY)) > 0) {
                    viewport = EE_MAX(0, ((int)viewport-(int)increment));
                    value->view(EPOSITION_POSITIONY, viewport);
                }
            } else {
                position[EPOSITION_POSITIONX] = destination(EPOSITION_POSITIONX);
                position[EPOSITION_POSITIONY] = (destination(EPOSITION_POSITIONY)+destination(EPOSITION_HEIGHT)-safesrc(interface[ESCROLLER_ARROW_DOWN], EPOSITION_HEIGHT));
                position[EPOSITION_WIDTH] =  safesrc(interface[ESCROLLER_ARROW_DOWN], EPOSITION_WIDTH);
                position[EPOSITION_HEIGHT] = safesrc(interface[ESCROLLER_ARROW_DOWN], EPOSITION_HEIGHT);
                inrange(mouse, position[EPOSITION_POSITIONX], position[EPOSITION_POSITIONY], position[EPOSITION_WIDTH], position[EPOSITION_HEIGHT], present);
                if (present) {
                    if ((viewport = value->view(EPOSITION_POSITIONY)) < EE_MAX(0, ((int)value->height()-(int)value->destination(EPOSITION_HEIGHT)))) {
                        viewport = EE_MIN(value->height(), ((int)viewport+(int)increment));
                        value->view(EPOSITION_POSITIONY, viewport);
                    }
                }
            }
        }
        mouse->positionx = absolute[EPOSITION_POSITIONX];
        mouse->positiony = absolute[EPOSITION_POSITIONY];
    } 
    value->forward(keyboard, mouse, positionx, positiony);
}
