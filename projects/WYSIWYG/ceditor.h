#ifndef CINTERFACE_H
#define	CINTERFACE_H
#include "cengine.h"

#include "canimation.h"
#include "centity.h"
#include "ccountdown.h"
#include "ceffects.h"
#include "cfx.h"
#include "ceditor.h"

#include "citemhandler.h"
#include "citem.h"
#define BUTTON_W 100
#define BUTTON_H 30
#define WINDOW_W 200
#define WINDOW_H 400
#define MARGIN_X 10
#define MARGIN_Y 10

const int NUM_BUTTON = 4;
class ceditor : public eagleclass {
public:
    cxml* editor;
    clabel* menulabel[NUM_BUTTON];
    cbutton* menubutton[NUM_BUTTON];
    cwindow* menuwindow[NUM_BUTTON];
    static bool masked_btn;
    void init();
    int loadxml();
    void forward (skeyboard *keyboard, smouse *mouse, unsigned int positionx, unsigned int positiony);
    static void load_fnc(cbutton *bttn, const char *identify);
    static void unload_fnc(cbutton *bttn, const char *identify);
    static inline void mask_fnc(cbutton *bttn, const char *identify){
        citemhandler::showlayer(ceditor::masked_btn, 0);
        masked_btn=!(masked_btn);
    }
    static inline void load2_fnc(cbutton *bttn, const char *identify){
        ctarga* context=NULL;
        if( canimation::loadmodel(&context, "torre.tga")){
//            elog("Loading item ok!");
            citem* itm=enew citem();
            itm->context=(ccontext*)context;
            citemhandler::add(itm, 1);
        }
//        else elog ("Loading error ... check filepath");
    }
    void checkselection (unsigned int positionx, unsigned int positiony, int layer);
    void dragselection (unsigned int curx, unsigned int cury, unsigned int precx, unsigned int precy, unsigned int layer);
    void drawmainmenu(char* label, char* func, int index, int layer);
    void drawwindow(char* label, int index, int layer);
    void drawcomponent(char* identify, int index);
    int conclusion();
private:
};

#endif	/* INTERFACE_H */

