/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CTHEME_H
#define EE_CTHEME_H
#include "cfilesystem.h"
#include "cxml.h"
#include "ctarga.h"
#include "cfonts.h"
enum ethelayout {
    ETHELAYOUT_COMPONENT_CORNER_NW = 0,
    ETHELAYOUT_COMPONENT_CORNER_NE,
    ETHELAYOUT_COMPONENT_CORNER_SW,
    ETHELAYOUT_COMPONENT_CORNER_SE,
    ETHELAYOUT_COMPONENT_BORDER_N,
    ETHELAYOUT_COMPONENT_BORDER_S,
    ETHELAYOUT_COMPONENT_BORDER_W,
    ETHELAYOUT_COMPONENT_BORDER_E,
    ETHELAYOUT_COMPONENT_BUTTON_LEFT,
    ETHELAYOUT_COMPONENT_BUTTON_MIDDLE,
    ETHELAYOUT_COMPONENT_BUTTON_RIGHT,
    ETHELAYOUT_COMPONENT_BUTTON_LEFT_PRESSED,
    ETHELAYOUT_COMPONENT_BUTTON_MIDDLE_PRESSED,
    ETHELAYOUT_COMPONENT_BUTTON_RIGHT_PRESSED,
    ETHELAYOUT_COMPONENT_SCROLLING_ARROW_UP,
    ETHELAYOUT_COMPONENT_SCROLLING_ARROW_DOWN,
    ETHELAYOUT_COMPONENT_SCROLLING_MIDDLE,
    ETHELAYOUT_COMPONENT_SCROLLING_SCROLLER,
    ETHELAYOUT_COMPONENT_BACKGROUND,
    ETHELAYOUT_WINDOW_CORNER_NW,
    ETHELAYOUT_WINDOW_CORNER_NE,
    ETHELAYOUT_WINDOW_CORNER_SW,
    ETHELAYOUT_WINDOW_CORNER_SE,
    ETHELAYOUT_WINDOW_BORDER_N,
    ETHELAYOUT_WINDOW_BORDER_S,
    ETHELAYOUT_WINDOW_BORDER_W,
    ETHELAYOUT_WINDOW_BORDER_E,
    ETHELAYOUT_WINDOW_BUTTON_CLOSE,
    ETHELAYOUT_WINDOW_BACKGROUND,
    ETHELAYOUT_IMAGE,
    ETHELAYOUT_FONT,
    ETHELAYOUT_ICON,
    ETHELAYOUT_NULL
};
const char cthecomponents[ETHELAYOUT_NULL][EE_LABEL_SIZE] = {
    "component.corner.nw",
    "component.corner.ne",
    "component.corner.sw",
    "component.corner.se",
    "component.border.n",
    "component.border.s",
    "component.border.w",
    "component.border.e",
    "component.button.left",
    "component.button.middle",
    "component.button.right",
    "component.button.left.pressed",
    "component.button.middle.pressed",
    "component.button.right.pressed",
    "component.scrolling.arrow.up",
    "component.scrolling.arrow.down",
    "component.scrolling.middle",
    "component.scrolling.scroller",
    "component.background",
    "window.corner.nw",
    "window.corner.ne",
    "window.corner.sw",
    "window.corner.se",
    "window.border.n",
    "window.border.s",
    "window.border.w",
    "window.border.e",
    "window.button.close",
    "window.background",
    "image.default",
    "font.default",
    "icon.default"
};
class ctheme:public eagleclass {
private:
    ctarga *components[ETHELAYOUT_NULL];
    cfonts *fonts;
public:
    inline ctheme (void) { this->init(); };
    void init (void);
    int initialize (cfilesystem *configuration);
    inline ctarga *get (ethelayout component) { return components[component]; };
    inline cfonts *get (void) { return fonts; };
    void unload (void);
};
#endif
