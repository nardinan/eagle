/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "cinput.h"
bool ekeys[EE_ASCII_SET];
void cinput::init (unsigned int pendings) {
    keyboard.init();
    mouse.init();
    this->pendings = pendings;
}

void cinput::reset (void) {
    int index;
    for (index = 0; index < EE_ASCII_SET; index++)
        ekeys[index] = false;
}

void cinput::analyze (einput kind, eaction action, int key, int positionx, int positiony) {
    switch (action) {
        case EACTION_PRESSED:
            press(kind, key, positionx, positiony);
            break;
        case EACTION_RELEASED:
            release(kind, key, positionx, positiony);
            break;
    }
    glutPostRedisplay();
}

void cinput::press (einput kind, int key, int positionx, int positiony) {
    skeyboard *keypressed = NULL;
    smouse *buttonpressed = NULL;
    switch (kind) {
        case EINPUT_KEYBOARD:
            if (keyboard.records() < pendings) {
                if (catcher[EINPUT_KEYBOARD]) {
                    if ((keypressed = (skeyboard *) emalloc(sizeof(skeyboard)))) {
                        keypressed->key = (unsigned char)key;
                        keypressed->positionx = positionx;
                        keypressed->positiony = positiony;
                        keypressed->action = EACTION_PRESSED;
                        keypressed->altmod = keypressed->ctrlmod = keypressed->shiftmod = false;
                        switch (glutGetModifiers()) {
                            case GLUT_ACTIVE_SHIFT:
                                keypressed->shiftmod = true;
                                break;
                            case GLUT_ACTIVE_CTRL:
                                keypressed->ctrlmod = true;
                                break;
                            case GLUT_ACTIVE_ALT:
                                keypressed->altmod = true;
                                break;
                        }
                        keyboard.add(keypressed); /* ignoring the result */
                        ekeys[keypressed->key] = true;
                    } else
                        ekill("out of memory");
                }
            }
            break;
        case EINPUT_SPECIAL:
            if (keyboard.records() < pendings) {
                if (catcher[EINPUT_SPECIAL]) {
                    if ((keypressed = (skeyboard *) emalloc(sizeof(skeyboard)))) {
                        keypressed->key = EKEYBOARD_NONE;
                        switch (key) {
                            case GLUT_KEY_UP: keypressed->key = EKEYBOARD_UP; break;
                            case GLUT_KEY_DOWN: keypressed->key = EKEYBOARD_DOWN; break;
                            case GLUT_KEY_RIGHT: keypressed->key = EKEYBOARD_RIGHT; break;
                            case GLUT_KEY_LEFT: keypressed->key = EKEYBOARD_LEFT; break;
                            case GLUT_KEY_F1: keypressed->key = EKEYBOARD_F1; break;
                            case GLUT_KEY_F2: keypressed->key = EKEYBOARD_F2; break;
                            case GLUT_KEY_F3: keypressed->key = EKEYBOARD_F3; break;
                            case GLUT_KEY_F4: keypressed->key = EKEYBOARD_F4; break;
                            case GLUT_KEY_F5: keypressed->key = EKEYBOARD_F5; break;
                            case GLUT_KEY_F6: keypressed->key = EKEYBOARD_F6; break;
                            case GLUT_KEY_F7: keypressed->key = EKEYBOARD_F7; break;
                            case GLUT_KEY_F8: keypressed->key = EKEYBOARD_F8; break;
                            case GLUT_KEY_F9: keypressed->key = EKEYBOARD_F9; break;
                            case GLUT_KEY_F10: keypressed->key = EKEYBOARD_F10; break;
                            case GLUT_KEY_F11: keypressed->key = EKEYBOARD_F11; break;
                            case GLUT_KEY_F12: keypressed->key = EKEYBOARD_F12; break;
                        }
                        keypressed->positionx = positionx;
                        keypressed->positiony = positiony;
                        keypressed->action = EACTION_PRESSED;
                        keypressed->altmod = keypressed->ctrlmod = keypressed->shiftmod = false;
                        switch (glutGetModifiers()) {
                            case GLUT_ACTIVE_SHIFT:
                                keypressed->shiftmod = true;
                                break;
                            case GLUT_ACTIVE_CTRL:
                                keypressed->ctrlmod = true;
                                break;
                            case GLUT_ACTIVE_ALT:
                                keypressed->altmod = true;
                                break;
                        }
                        keyboard.add(keypressed); /* ignoring the result */
                        ekeys[keypressed->key] = true;
                    } else
                        ekill("out of memory");
                }
            }
            break;
        case EINPUT_MOUSE:
            if (mouse.records() < pendings) {
                if (catcher[EINPUT_MOUSE]) {
                    if ((buttonpressed = (smouse *) emalloc(sizeof(smouse)))) {
                        switch(key) {
                            case GLUT_LEFT_BUTTON: buttonpressed->key = EMOUSE_LEFT; break;
                            case GLUT_RIGHT_BUTTON: buttonpressed->key = EMOUSE_RIGHT; break;
                            case GLUT_MIDDLE_BUTTON: buttonpressed->key = EMOUSE_MIDDLE; break;
                        }
                        buttonpressed->positionx = positionx;
                        buttonpressed->positiony = positiony;
                        buttonpressed->action = EACTION_PRESSED;
                        buttonpressed->altmod = buttonpressed->ctrlmod = buttonpressed->shiftmod = false;
                        switch (glutGetModifiers()) {
                            case GLUT_ACTIVE_SHIFT:
                                buttonpressed->shiftmod = true;
                                break;
                            case GLUT_ACTIVE_CTRL:
                                buttonpressed->ctrlmod = true;
                                break;
                            case GLUT_ACTIVE_ALT:
                                buttonpressed->altmod = true;
                                break;
                        }
                        mouse.add(buttonpressed); /* ignoring the result */
                    } else
                        ekill("out of memory");
                }
            }
        default:
            break;
    }
}

void cinput::release (einput kind, int key, int positionx, int positiony) {
    skeyboard *keypressed = NULL;
    smouse *buttonpressed = NULL;
    switch (kind) {
        case EINPUT_KEYBOARD:
            if (keyboard.records() < pendings) {
                if (catcher[EINPUT_KEYBOARD]) {
                    if ((keypressed = (skeyboard *) emalloc(sizeof(skeyboard)))) {
                        keypressed->key = (unsigned char)key;
                        keypressed->positionx = positionx;
                        keypressed->positiony = positiony;
                        keypressed->action = EACTION_RELEASED;
                        keypressed->altmod = keypressed->ctrlmod = keypressed->shiftmod = false;
                        switch (glutGetModifiers()) {
                            case GLUT_ACTIVE_SHIFT:
                                keypressed->shiftmod = true;
                                break;
                            case GLUT_ACTIVE_CTRL:
                                keypressed->ctrlmod = true;
                                break;
                            case GLUT_ACTIVE_ALT:
                                keypressed->altmod = true;
                                break;
                        }
                        keyboard.add(keypressed); /* ignoring the result */
                        ekeys[keypressed->key] = false;
                    } else
                        ekill("out of memory");
                }
            }
            break;
        case EINPUT_SPECIAL:
            if (keyboard.records() < pendings) {
                if (catcher[EINPUT_SPECIAL]) {
                    if ((keypressed = (skeyboard *) emalloc(sizeof(skeyboard)))) {
                        keypressed->key = EKEYBOARD_NONE;
                        switch (key) {
                            case GLUT_KEY_UP: keypressed->key = EKEYBOARD_UP; break;
                            case GLUT_KEY_DOWN: keypressed->key = EKEYBOARD_DOWN; break;
                            case GLUT_KEY_RIGHT: keypressed->key = EKEYBOARD_RIGHT; break;
                            case GLUT_KEY_LEFT: keypressed->key = EKEYBOARD_LEFT; break;
                            case GLUT_KEY_F1: keypressed->key = EKEYBOARD_F1; break;
                            case GLUT_KEY_F2: keypressed->key = EKEYBOARD_F2; break;
                            case GLUT_KEY_F3: keypressed->key = EKEYBOARD_F3; break;
                            case GLUT_KEY_F4: keypressed->key = EKEYBOARD_F4; break;
                            case GLUT_KEY_F5: keypressed->key = EKEYBOARD_F5; break;
                            case GLUT_KEY_F6: keypressed->key = EKEYBOARD_F6; break;
                            case GLUT_KEY_F7: keypressed->key = EKEYBOARD_F7; break;
                            case GLUT_KEY_F8: keypressed->key = EKEYBOARD_F8; break;
                            case GLUT_KEY_F9: keypressed->key = EKEYBOARD_F9; break;
                            case GLUT_KEY_F10: keypressed->key = EKEYBOARD_F10; break;
                            case GLUT_KEY_F11: keypressed->key = EKEYBOARD_F11; break;
                            case GLUT_KEY_F12: keypressed->key = EKEYBOARD_F12; break;
                        }
                        keypressed->positionx = positionx;
                        keypressed->positiony = positiony;
                        keypressed->action = EACTION_RELEASED;
                        keypressed->altmod = keypressed->ctrlmod = keypressed->shiftmod = false;
                        switch (glutGetModifiers()) {
                            case GLUT_ACTIVE_SHIFT:
                                keypressed->shiftmod = true;
                                break;
                            case GLUT_ACTIVE_CTRL:
                                keypressed->ctrlmod = true;
                                break;
                            case GLUT_ACTIVE_ALT:
                                keypressed->altmod = true;
                                break;
                        }
                        keyboard.add(keypressed); /* ignoring the result */
                        ekeys[keypressed->key] = false;
                    } else
                        ekill("out of memory");
                }
            }
            break;
        case EINPUT_MOUSE:
            if (mouse.records() < pendings) {
                if (catcher[EINPUT_MOUSE]) {
                    if ((buttonpressed = (smouse *) emalloc(sizeof(smouse)))) {
                        switch(key) {
                            case GLUT_LEFT_BUTTON: buttonpressed->key = EMOUSE_LEFT; break;
                            case GLUT_RIGHT_BUTTON: buttonpressed->key = EMOUSE_RIGHT; break;
                            case GLUT_MIDDLE_BUTTON: buttonpressed->key = EMOUSE_MIDDLE; break;
                        }
                        buttonpressed->positionx = positionx;
                        buttonpressed->positiony = positiony;
                        buttonpressed->action = EACTION_RELEASED;
                        buttonpressed->altmod = buttonpressed->ctrlmod = buttonpressed->shiftmod = false;
                        switch (glutGetModifiers()) {
                            case GLUT_ACTIVE_SHIFT:
                                buttonpressed->shiftmod = true;
                                break;
                            case GLUT_ACTIVE_CTRL:
                                buttonpressed->ctrlmod = true;
                                break;
                            case GLUT_ACTIVE_ALT:
                                buttonpressed->altmod = true;
                                break;
                        }
                        mouse.add(buttonpressed); /* ignoring the result */
                    } else
                        ekill("out of memory");
                }
            }
        default:
            break;
    }
}

void cinput::motion (int positionx, int positiony, int resolutionx, int resolutiony) {
    mousex = ((positionx>(int)resolutionx)?resolutionx:EE_MAX(0, positionx));
    mousey = ((positiony>(int)resolutiony)?resolutiony:EE_MAX(0, positiony));
}

skeyboard *cinput::key (void) {
    skeyboard *keypressed = keyboard.get((size_t)0);
    if (keypressed)
        keyboard.del();
    return keypressed;
}

smouse *cinput::button (void) {
    smouse *buttonpressed = mouse.get((size_t)0);
    if (buttonpressed)
        mouse.del();
    return buttonpressed;
}

void cinput::unload (void) {
    destroy(&keyboard, efree, skeyboard);
    destroy(&mouse, efree, smouse);
    this->init();
}
