/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Tullio Guli
 * EMAIL        : gulitullio@gmail.com
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */

#include <ctype.h>
#include <math.h>
#include "cengine.h"
#include "canimation.h"
#include "centity.h"
#include "ccountdown.h"
#include "ceffects.h"
#include "cfx.h"
#include "ceditor.h"
#define EE_PRINT
ceditor* editor = NULL;
ccountdown* timer = NULL;
unsigned int precx = 0, precy;
bool drag = false;
            
int conclusion(void) {
    editor->conclusion();
    delete(editor);
    timer->unload();
    delete(timer);
    return EE_OK;
}

int loop(void) {
    int result = EE_OK;
    skeyboard *key = cengine::inputs.key();
    smouse *mouse = cengine::inputs.button();
    unsigned int curx = cengine::inputs.positionx();
    unsigned int cury = cengine::inputs.positiony();
    if (key) { 
        if (key->key == (char) 27) {
            result = EE_DONE;
        }
        efree(key);
    }
    if (mouse) {
        if (editor) {
            editor->forward(NULL,mouse,mouse->positionx,mouse->positiony);
        }    
        if (mouse->key == EMOUSE_LEFT) {
            if (mouse->action == EACTION_RELEASED) {
                editor->checkselection(mouse->positionx,mouse->positiony,0);
                drag = false;
            } else if (mouse->action == EACTION_PRESSED) {
                drag = true;
            }
        }
        efree(mouse);
    }
    if (timer->check("dragselection",15)) {
        if (drag) editor->dragselection(curx,cury,precx,precy,0);
        precx = curx;
        precy = cury;
    }     

    return result;
}

int main(int argc, char *argv[]) {
    if ((cengine::initialize("./default.config.xml", &argc, argv)) == EE_OK) {
        if ((cengine::initialize("./default.theme.dgt")) == EE_OK) {
            cengine::gameloop(loop);
            cengine::gamequit(conclusion);
            cengine::inputs.listening(EINPUT_KEYBOARD, true);
            cengine::inputs.listening(EINPUT_SPECIAL, true);
            cengine::inputs.listening(EINPUT_MOUSE, true);
            
            if (editor = enew ceditor()) {
                editor->init();
            }
            if (timer = enew ccountdown()) {
                timer->add("dragselection");
            }
            
            cengine::start();
        }
    }
    return 0;
}