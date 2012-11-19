/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "cfonts.h"
ccontext *cfonts::replacement = NULL;
void cfonts::init (void) {
    fonts.init();
    width = height = 0;
}

int cfonts::add (ccontext *layer, const char *label) {
    sfont *font = NULL;
    scolor *pixel = NULL;
    unsigned int positionx = 0;
    bool pinking = true;
    int result = EE_OK;
    /* warning default replacement */
    if (!layer) {
        if (cfonts::replacement)
            layer = cfonts::replacement;
        else
            result = EE_ERROR;
    }
    /* end */
    if (result) {
        if ((font = (sfont *) emalloc(sizeof(sfont)))) {
            font->last = 0;
            font->layer = layer;
            while ((positionx < layer->source(EPOSITION_WIDTH)) && (font->last < EE_CHARACTERS)) {
                if ((pixel = layer->pixel(positionx, 0))) {
                    if (!pinking) {
                        if (pink(pixel)) {
                            font->offset[font->last++] = positionx;
                            pinking = true;
                        }
                    } else if (!pink(pixel)) {
                        font->offset[font->last++] = positionx;
                        pinking = false;
                    }
                    efree(pixel);
                }
                positionx++;
            }
            font->offset[font->last] = (layer->source(EPOSITION_WIDTH)-1);
            font->space = font->offset[font->last]-font->offset[font->last-1];
            font->channels.color[ECOLOR_R] = font->channels.color[ECOLOR_G] = font->channels.color[ECOLOR_B] = font->channels.color[ECOLOR_A] = 1.0f;
            if ((result = analyze(font)))
                result = fonts.add(font, label);
        } else
            ekill("out of memory");
    }
    return result;
}

int cfonts::analyze (sfont *font) {
    int offset, result = EE_OK;
    for (offset = 0; offset < font->last; offset += 2) {
        font->positions[offset/2][EPOSITION_POSITIONY] = 1;
        font->positions[offset/2][EPOSITION_POSITIONX] = font->offset[offset];
        font->positions[offset/2][EPOSITION_HEIGHT] = font->layer->source(EPOSITION_HEIGHT)-1;
        font->positions[offset/2][EPOSITION_WIDTH] = (font->offset[offset+1]-font->offset[offset]);
    }
    return result;
}

void cfonts::del (const char *label) {
    sfont *current;
    if ((current = fonts.get(label))) {
        efree(current);
        fonts.del();
    }
}

void cfonts::unload (void) {
    destroy(&fonts, efree, sfont);
    this->init();
}
