/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "cwindow.h"
ewindow& operator++ (ewindow& value) { return value = (value==EWINDOW_NULL)?EWINDOW_BACKGROUND:ewindow(value+1); }
void cwindow::init (void) {
    int index;
    ccomponent::init(); /* father's init */
    components.init();
    for (index = 0; index < EWINDOW_NULL; index++)
        interface[index] = NULL;
    current = EWINDOW_NULL;
    anchored = selected = false;
    visible = true;
    background = EE_WINDOW_BACKGROUND;
    drop(EPOSITION_POSITIONX, 0);
    drop(EPOSITION_POSITIONY, 0);
    drop(EPOSITION_WIDTH, EE_WINDOW_W);
    drop(EPOSITION_HEIGHT, EE_WINDOW_H);
}

int cwindow::initialize (ctheme *theme) {
    int index, result = EE_OK;
    ethelayout component = ETHELAYOUT_NULL;
    for (index = 0; index < EWINDOW_NULL; index++) {
        switch (index) {
            case EWINDOW_CORNER_NW: component = ETHELAYOUT_WINDOW_CORNER_NW; break;
            case EWINDOW_CORNER_NE: component = ETHELAYOUT_WINDOW_CORNER_NE; break;
            case EWINDOW_CORNER_SW: component = ETHELAYOUT_WINDOW_CORNER_SW; break;
            case EWINDOW_CORNER_SE: component = ETHELAYOUT_WINDOW_CORNER_SE; break;
            case EWINDOW_BORDER_N: component = ETHELAYOUT_WINDOW_BORDER_N; break;
            case EWINDOW_BORDER_S: component = ETHELAYOUT_WINDOW_BORDER_S; break;
            case EWINDOW_BORDER_W: component = ETHELAYOUT_WINDOW_BORDER_W; break;
            case EWINDOW_BORDER_E: component = ETHELAYOUT_WINDOW_BORDER_E; break;
            case EWINDOW_BUTTON_CLOSE: component = ETHELAYOUT_WINDOW_BUTTON_CLOSE; break;
            case EWINDOW_BACKGROUND: component = ETHELAYOUT_WINDOW_BACKGROUND; break;
        }
        interface[index] = theme->get(component);
    }
    return result;
}

void cwindow::rebuild (ewindow component) {
    int final[EPOSITION_NULL];
    if (interface[component]) {
        restore();
        final[EPOSITION_POSITIONX] = destination(EPOSITION_POSITIONX);
        final[EPOSITION_POSITIONY] = destination(EPOSITION_POSITIONY);
        final[EPOSITION_WIDTH] = interface[component]->source(EPOSITION_WIDTH);
        final[EPOSITION_HEIGHT] = interface[component]->source(EPOSITION_HEIGHT);
        switch (component) {
            case EWINDOW_CORNER_NW:
                break;
            case EWINDOW_CORNER_NE:
                final[EPOSITION_POSITIONX] = (destination(EPOSITION_POSITIONX)+destination(EPOSITION_WIDTH)-safesrc(interface[EWINDOW_CORNER_NE], EPOSITION_WIDTH));
                break;
            case EWINDOW_CORNER_SW:
                final[EPOSITION_POSITIONY] = (destination(EPOSITION_POSITIONY)+destination(EPOSITION_HEIGHT)-safesrc(interface[EWINDOW_CORNER_SW], EPOSITION_HEIGHT));
                break;
            case EWINDOW_CORNER_SE:
                final[EPOSITION_POSITIONX] = (destination(EPOSITION_POSITIONX)+destination(EPOSITION_WIDTH)-safesrc(interface[EWINDOW_CORNER_SE], EPOSITION_WIDTH));
                final[EPOSITION_POSITIONY] = (destination(EPOSITION_POSITIONY)+destination(EPOSITION_HEIGHT)-safesrc(interface[EWINDOW_CORNER_SE], EPOSITION_HEIGHT));
                break;
            case EWINDOW_BORDER_N:
                final[EPOSITION_POSITIONX] = (destination(EPOSITION_POSITIONX)+interface[EWINDOW_CORNER_NW]->source(EPOSITION_WIDTH));
                final[EPOSITION_WIDTH] = EE_MAX(0, (destination(EPOSITION_WIDTH)-safesrc(interface[EWINDOW_CORNER_NW], EPOSITION_WIDTH)-safesrc(interface[EWINDOW_CORNER_NE], EPOSITION_WIDTH)));
                break;
            case EWINDOW_BORDER_S:
                final[EPOSITION_POSITIONX] = (destination(EPOSITION_POSITIONX)+safesrc(interface[EWINDOW_CORNER_SW], EPOSITION_WIDTH));
                final[EPOSITION_POSITIONY] =  (destination(EPOSITION_POSITIONY)+destination(EPOSITION_HEIGHT)-safesrc(interface[EWINDOW_CORNER_SW], EPOSITION_HEIGHT));
                final[EPOSITION_WIDTH] = EE_MAX(0, (destination(EPOSITION_WIDTH)-safesrc(interface[EWINDOW_CORNER_SW], EPOSITION_WIDTH)-safesrc(interface[EWINDOW_CORNER_SE], EPOSITION_WIDTH)));
                break;
            case EWINDOW_BORDER_W:
                final[EPOSITION_POSITIONY] = (destination(EPOSITION_POSITIONY)+safesrc(interface[EWINDOW_CORNER_NW], EPOSITION_HEIGHT));
                final[EPOSITION_HEIGHT] = EE_MAX(0, (destination(EPOSITION_HEIGHT)-safesrc(interface[EWINDOW_CORNER_NW], EPOSITION_HEIGHT)-safesrc(interface[EWINDOW_CORNER_NE], EPOSITION_HEIGHT)));
                break;
            case EWINDOW_BORDER_E:
                final[EPOSITION_POSITIONX] = (destination(EPOSITION_POSITIONX)+destination(EPOSITION_WIDTH)-safesrc(interface[EWINDOW_CORNER_SE], EPOSITION_WIDTH));
                final[EPOSITION_POSITIONY] = (destination(EPOSITION_POSITIONY)+safesrc(interface[EWINDOW_CORNER_NW], EPOSITION_HEIGHT));
                final[EPOSITION_HEIGHT] = EE_MAX(0, (destination(EPOSITION_HEIGHT)-safesrc(interface[EWINDOW_CORNER_NW], EPOSITION_HEIGHT)-safesrc(interface[EWINDOW_CORNER_NE], EPOSITION_HEIGHT)));
                break;
            case EWINDOW_BUTTON_CLOSE:
                final[EPOSITION_POSITIONX] = (destination(EPOSITION_POSITIONX)+safesrc(interface[EWINDOW_BUTTON_CLOSE], EPOSITION_WIDTH)/2);
                final[EPOSITION_POSITIONY] = (destination(EPOSITION_POSITIONY)+safesrc(interface[EWINDOW_BUTTON_CLOSE], EPOSITION_HEIGHT)/2);
                break;
            case EWINDOW_BACKGROUND:
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

GLuint cwindow::get (void) {
    GLuint result = EE_NULLTEXTURE;
    ccontext *component = components.get();
    if (visible) {
        if (current == EWINDOW_NULL) {
            if (!component)
                current = (background)?EWINDOW_BACKGROUND:EWINDOW_NULL;
        } else
            current++;
        if ((current != EWINDOW_NULL) && (interface[current])) {
            rebuild((ewindow)current);
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
            if (!component)
                component = components.nextvalue();
            while (component) {
                if ((result = component->get()) == EE_NULLTEXTURE)
                    component = components.nextvalue();
                else
                    break;
            }
            if (result == EE_NULLTEXTURE)
                current = EWINDOW_NULL;
            else {                
                coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_POSITIONX] = component->destination(EPOSITION_POSITIONX, EPOSITIONSUBJECT_FRAME)+destination(EPOSITION_POSITIONX);
                coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_POSITIONY] = component->destination(EPOSITION_POSITIONY, EPOSITIONSUBJECT_FRAME)+destination(EPOSITION_POSITIONY)+safesrc(interface[EWINDOW_BORDER_N], EPOSITION_HEIGHT);
                coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_WIDTH] = component->destination(EPOSITION_WIDTH, EPOSITIONSUBJECT_FRAME);
                coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_HEIGHT] = component->destination(EPOSITION_HEIGHT, EPOSITIONSUBJECT_FRAME);
                coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_POSITIONX] = component->source(EPOSITION_POSITIONX, EPOSITIONSUBJECT_FRAME);
                coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_POSITIONY] = component->source(EPOSITION_POSITIONY, EPOSITIONSUBJECT_FRAME);
                coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_WIDTH] = component->source(EPOSITION_WIDTH, EPOSITIONSUBJECT_FRAME);
                coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_HEIGHT] = component->source(EPOSITION_HEIGHT, EPOSITIONSUBJECT_FRAME);
                flipped[EFLIPDIRECTION_HORIZONTAL] = component->flipped[EFLIPDIRECTION_HORIZONTAL];
                flipped[EFLIPDIRECTION_VERTICAL] = component->flipped[EFLIPDIRECTION_VERTICAL];
                channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_R] = mastercolor(component->gloss(ECOLOR_R, EPOSITIONSUBJECT_FRAME), channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_R]);
                channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_G] = mastercolor(component->gloss(ECOLOR_G, EPOSITIONSUBJECT_FRAME), channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_G]);
                channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_B] = mastercolor(component->gloss(ECOLOR_B, EPOSITIONSUBJECT_FRAME), channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_B]);
                channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_A] = mastercolor(component->gloss(ECOLOR_A, EPOSITIONSUBJECT_FRAME), channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_A]);
            }
            /* end */
        } 
    } else {
        current = EWINDOW_NULL;
        components.reset();
    }
    return result;
}

void cwindow::restore (void) {
    int index;
    for (index = 0; index < EWINDOW_NULL; index++)
        if (interface[index])
            interface[index]->restore();
    channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_R] = channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_G] = channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_B] = channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_A] = 1.0f;
    channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_R] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_G] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_B] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_A] = 1.0f;
    rotation = 0.0f;
}

void cwindow::forward (skeyboard *keyboard, smouse *mouse, unsigned int positionx, unsigned int positiony) {
    int final[EPOSITION_NULL];
    ccomponent *component;
    bool closing;
    if (visible) {
        if (mouse) {
            if (mouse->action == EACTION_PRESSED) {
                if (interface[EWINDOW_BUTTON_CLOSE]) {
                    rebuild(EWINDOW_BUTTON_CLOSE);
                    final[EPOSITION_POSITIONX] = EE_MAX(destination(EPOSITION_POSITIONX), safedst(interface[EWINDOW_BUTTON_CLOSE], EPOSITION_POSITIONX));
                    final[EPOSITION_POSITIONY] = EE_MAX(destination(EPOSITION_POSITIONY), safedst(interface[EWINDOW_BUTTON_CLOSE], EPOSITION_POSITIONY));
                    final[EPOSITION_WIDTH] = safesrc(interface[EWINDOW_BUTTON_CLOSE], EPOSITION_WIDTH);
                    final[EPOSITION_HEIGHT] = safesrc(interface[EWINDOW_BUTTON_CLOSE], EPOSITION_HEIGHT);
                    inrange(mouse, final[EPOSITION_POSITIONX], final[EPOSITION_POSITIONY], final[EPOSITION_WIDTH], final[EPOSITION_HEIGHT], closing);
                    if (closing)
                        visibility(false);
                }
                if (visible) {
                    final[EPOSITION_POSITIONX] = destination(EPOSITION_POSITIONX);
                    final[EPOSITION_POSITIONY] = destination(EPOSITION_POSITIONY);
                    final[EPOSITION_WIDTH] = destination(EPOSITION_WIDTH);
                    final[EPOSITION_HEIGHT] = EE_MAX(5, safesrc(interface[EWINDOW_BORDER_N], EPOSITION_HEIGHT));
                    inrange(mouse, final[EPOSITION_POSITIONX], final[EPOSITION_POSITIONY], final[EPOSITION_WIDTH], final[EPOSITION_HEIGHT], selected);
                    if (anchored)
                        selected = false;
                    else {
                        overflow[EPOSITION_POSITIONX] = EE_MAX(0, ((int)positionx-destination(EPOSITION_POSITIONX)));
                        overflow[EPOSITION_POSITIONY] = EE_MAX(0, ((int)positiony-destination(EPOSITION_POSITIONY)));
                    }
                }
            } else
                selected = false;
        }
        if (selected) {
            drop(EPOSITION_POSITIONX, EE_MAX(0, ((int)positionx-overflow[EPOSITION_POSITIONX])));
            drop(EPOSITION_POSITIONY, EE_MAX(0, ((int)positiony-overflow[EPOSITION_POSITIONY])));
        }
        foreach(component, &components)
            component->forward(keyboard, mouse, (positionx-destination(EPOSITION_POSITIONX)), (positiony-destination(EPOSITION_POSITIONY)-safesrc(interface[EWINDOW_BORDER_N], EPOSITION_HEIGHT)));
    }    
}

void cwindow::unload (void) {
    components.unload();
    this->init();
}
