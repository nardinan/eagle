/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "ctheme.h"
ethelayout& operator++ (ethelayout& value) { return value = (value==ETHELAYOUT_NULL)?ETHELAYOUT_COMPONENT_CORNER_NW:ethelayout(value+1); }
void ctheme::init (void) {
    int index;
    for (index = 0; index < ETHELAYOUT_NULL; index++)
        components[index] = NULL;
    fonts = NULL;
}

int ctheme::initialize (cfilesystem *configuration) {
    sdfssegment *segment;
    int result = EE_OK, index;
    for (index = 0; index < ETHELAYOUT_NULL; index++) {
        /* sorry but here, you can't count on ctarga replacement! */
        if ((segment = configuration->get(cthecomponents[index]))) {
            if ((components[index] = enew ctarga())) {
                result = components[index]->initialize(segment);
            } else
                ekill("out of memory");
        } else
            result = EE_ERROR;
        if (!result)
            break;
    }
    if (components[ETHELAYOUT_FONT])
        if ((fonts = enew cfonts())) {
            if ((result = fonts->add(components[ETHELAYOUT_FONT], "default")))
                if (components[ETHELAYOUT_ICON])
                    result = fonts->add(components[ETHELAYOUT_ICON], "icons");
        } else
            ekill("out of memory");
    return result;
}

void ctheme::unload (void) {
    int index;
    for (index = 0; index < ETHELAYOUT_NULL; index++)
        if (components[index]) {
            components[index]->unload();
            delete(components[index]);
        }
    if (fonts) {
        fonts->unload();
        delete(fonts);
    }
    init();
}
