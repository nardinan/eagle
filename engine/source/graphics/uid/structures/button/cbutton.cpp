/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "cbutton.h"
ebutton& operator++ (ebutton& value) { return value = (value==EBUTTON_NULL)?EBUTTON_LEFT:ebutton(value+1); }
void cbutton::init (ccontext *context) {
    int index;
    ccomponent::init(); /* father's init */
    for (index = 0; index < EBUTTON_NULL; index++)
        interface[index] = NULL;
    handler = NULL;
    value = context;
    current = EBUTTON_NULL;
    components = pressed = selected = false;
    visible = true;
    background = EE_BUTTON_BACKGROUND;
    drop(EPOSITION_WIDTH, EE_BUTTON_W);
}

int cbutton::initialize (ctheme *theme) {
    int index, result = EE_OK;
    ethelayout component = ETHELAYOUT_NULL;
    for (index = 0; index < EBUTTON_NULL; index++) {
        switch (index) {
            case EBUTTON_LEFT: component = ETHELAYOUT_COMPONENT_BUTTON_LEFT; break;
            case EBUTTON_MIDDLE: component = ETHELAYOUT_COMPONENT_BUTTON_MIDDLE; break;
            case EBUTTON_RIGHT: component = ETHELAYOUT_COMPONENT_BUTTON_RIGHT; break;
            case EBUTTON_LEFT_PRESSED: component = ETHELAYOUT_COMPONENT_BUTTON_LEFT_PRESSED; break;
            case EBUTTON_MIDDLE_PRESSED: component = ETHELAYOUT_COMPONENT_BUTTON_MIDDLE_PRESSED; break;
            case EBUTTON_RIGHT_PRESSED: component = ETHELAYOUT_COMPONENT_BUTTON_RIGHT_PRESSED; break;
        }
        interface[index] = theme->get(component);
    }
    return result;
}

void cbutton::handleizer (const char *identifier, fbuttonhandler *handler) {
    strncpy(this->identifier, identifier, EE_LABEL_SIZE);
    this->identifier[EE_MIN(estrlen(identifier), (EE_LABEL_SIZE-1))] = '\0';
    this->handler = handler;
}

void cbutton::rebuild (ebutton component) {
    int final[EPOSITION_NULL];
    if (interface[component]) {
        restore();
        final[EPOSITION_POSITIONX] = destination(EPOSITION_POSITIONX);
        final[EPOSITION_POSITIONY] = destination(EPOSITION_POSITIONY);
        final[EPOSITION_WIDTH] = interface[component]->source(EPOSITION_WIDTH);
        final[EPOSITION_HEIGHT] = interface[component]->source(EPOSITION_HEIGHT);
        switch (component) {
            case EBUTTON_LEFT:
            case EBUTTON_LEFT_PRESSED:
                break;
            case EBUTTON_MIDDLE:
            case EBUTTON_MIDDLE_PRESSED:
                final[EPOSITION_POSITIONX] = (destination(EPOSITION_POSITIONX)+safesrc(interface[EBUTTON_LEFT], EPOSITION_WIDTH));
                final[EPOSITION_WIDTH] = EE_MAX(0, (destination(EPOSITION_WIDTH)-safesrc(interface[EBUTTON_LEFT], EPOSITION_WIDTH)-safesrc(interface[EBUTTON_RIGHT], EPOSITION_WIDTH)));
                break;
            case EBUTTON_RIGHT:
            case EBUTTON_RIGHT_PRESSED:
                final[EPOSITION_POSITIONX] = (destination(EPOSITION_POSITIONX)+destination(EPOSITION_WIDTH)-safesrc(interface[EBUTTON_RIGHT], EPOSITION_WIDTH));
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

GLuint cbutton::get (void) {
    GLuint result = EE_NULLTEXTURE;
    if (visible) {
        if ((current == EBUTTON_NULL) || ((!pressed) && (current == EBUTTON_LEFT_PRESSED))) {
            if (!components) {
                if (pressed)
                    current = (background)?EBUTTON_LEFT_PRESSED:EBUTTON_NULL;
                else
                    current = (background)?EBUTTON_LEFT:EBUTTON_NULL;
            }
        } else
            current++;
        if (((current != EBUTTON_NULL) && ((pressed) || (current != EBUTTON_LEFT_PRESSED))) && (interface[current])) {
            rebuild((ebutton)current);
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
                coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_POSITIONX] = value->destination(EPOSITION_POSITIONX, EPOSITIONSUBJECT_FRAME)+destination(EPOSITION_POSITIONX)+EE_MAX(0, ((int)(destination(EPOSITION_WIDTH)/2)-(int)(value->source(EPOSITION_WIDTH)/2))); /* centered */
                coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_POSITIONY] = value->destination(EPOSITION_POSITIONY, EPOSITIONSUBJECT_FRAME)+destination(EPOSITION_POSITIONY);
                coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_WIDTH] = value->destination(EPOSITION_WIDTH, EPOSITIONSUBJECT_FRAME);
                coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_HEIGHT] = value->destination(EPOSITION_HEIGHT, EPOSITIONSUBJECT_FRAME);
                coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_POSITIONX] = value->source(EPOSITION_POSITIONX, EPOSITIONSUBJECT_FRAME);
                coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_POSITIONY] = value->source(EPOSITION_POSITIONY, EPOSITIONSUBJECT_FRAME);
                coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_WIDTH] = value->source(EPOSITION_WIDTH, EPOSITIONSUBJECT_FRAME);
                coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_HEIGHT] = value->source(EPOSITION_HEIGHT, EPOSITIONSUBJECT_FRAME);
                flipped[EFLIPDIRECTION_HORIZONTAL] = value->flipped[EFLIPDIRECTION_HORIZONTAL];
                flipped[EFLIPDIRECTION_VERTICAL] = value->flipped[EFLIPDIRECTION_VERTICAL];
                channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_R] = mastercolor(value->gloss(ECOLOR_R, EPOSITIONSUBJECT_FRAME), channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_R]);
                channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_G] = mastercolor(value->gloss(ECOLOR_G, EPOSITIONSUBJECT_FRAME), channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_G]);
                channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_B] = mastercolor(value->gloss(ECOLOR_B, EPOSITIONSUBJECT_FRAME), channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_B]);
                channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_A] = mastercolor(value->gloss(ECOLOR_A, EPOSITIONSUBJECT_FRAME), channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_A]);
            } else
                components = false;
        }
    }
    return result;
}
 
void cbutton::restore (void) {
    int index;
    for (index = 0; index < EBUTTON_NULL; index++)
        if (interface[index])
            interface[index]->restore();
    channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_R] = channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_G] = channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_B] = channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_A] = 1.0f;
    channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_R] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_G] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_B] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_A] = 1.0f;
    rotation = 0.0f;
}

void cbutton::forward (skeyboard *keyboard, smouse *mouse, unsigned int positionx, unsigned int positiony) {
    bool present;
    int absolute[EPOSITION_NULL];
    if (mouse) {
        absolute[EPOSITION_POSITIONX] = mouse->positionx;
        absolute[EPOSITION_POSITIONY] = mouse->positiony;
        mouse->positionx = positionx;
        mouse->positiony = positiony;
        inrange(mouse, destination(EPOSITION_POSITIONX), destination(EPOSITION_POSITIONY), destination(EPOSITION_WIDTH), safesrc(interface[EBUTTON_MIDDLE], EPOSITION_HEIGHT), present);
        if (mouse->action == EACTION_PRESSED) {
            pressed = present;
        } else {
            if (pressed) {
                if ((present) && (handler))
                    handler(this, identifier);
                pressed = false;
            }
        }
        mouse->positionx = absolute[EPOSITION_POSITIONX];
        mouse->positiony = absolute[EPOSITION_POSITIONY];
    }
}

void cbutton::unload (void) {
    this->init(value);
}
