/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Tullio Guli
 * EMAIL        : gulitullio@gmail.com
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */

#include "ceditor.h"
bool ceditor::masked_btn;

void ceditor::init() {
    ceditor::loadxml();
    //menuwindow[1]->visibility(true);
}

int ceditor::loadxml() {
    
    //INIZIALIZZAZIONE CITEMHANDLER
    citemhandler::init();
    
    
    editor = enew cxml();
    sxmlclass *xmlblock[2] = {NULL, NULL};
    if (cengine::filesystem.add("editor.xml", "editor")) {
        if (editor->analyze(cengine::filesystem.get("editor"))) {
            if (xmlblock[0] = editor->get("s", "editor")) {
//                elog("%d", xmlblock[0]->subclass.records());
                foreach(xmlblock[1], &xmlblock[0]->subclass) {
//                    elog("%d %d", xmlblock[0],xmlblock[1]);
                    ceditor::drawmainmenu(xmlblock[1]->keys.get("ID"), xmlblock[1]->keys.get("FUNCTION"), atoi(xmlblock[1]->keys.get("INDEX")),1);
                    int count = xmlblock[1]->subclass.records();
//                    elog("%d",count);
                }
            }
        }
    }
    return EE_OK;
}

void ceditor::drawmainmenu(char* label, char* func, int i, int layer) {
    if (menulabel[i] = enew clabel()) {
        menulabel[i]->initialize(&cengine::theme);
        menulabel[i]->content(label);
        setx(menulabel[i],-4);
        if (menubutton[i] = enew cbutton(menulabel[i])) {
            menubutton[i]->initialize(&cengine::theme);
            setw(menubutton[i],BUTTON_W);
            seth(menubutton[i],BUTTON_H);
            setx(menubutton[i],i*BUTTON_W+(i+1)*MARGIN_X);
            sety(menubutton[i],MARGIN_Y);

            if (strcmp(func,"load_fnc")==0) menubutton[i]->handleizer(label, ceditor::load_fnc);
            else if (strcmp(func,"mask_fnc")==0) {
                ceditor::masked_btn=false;
                menubutton[i]->handleizer(label, ceditor::mask_fnc);
            } 
            else if (strcmp(func,"unload_fnc")==0) menubutton[i]->handleizer(label, ceditor::unload_fnc);
            else if (strcmp(func,"load2_fnc")==0) menubutton[i]->handleizer(label, ceditor::load2_fnc);
            
            cengine::environment[EENVIRONMENT_OVER].add(menubutton[i],layer);
            ceditor::drawwindow(label,i,1);
            //ceditor::drawcomponent(label,i);
        }
    }
}

void ceditor::drawwindow(char* id, int i, int layer) {
    if (menuwindow[i] = enew cwindow()) {
        menuwindow[i]->initialize(&cengine::theme);
        setw(menuwindow[i],WINDOW_W);
        seth(menuwindow[i],WINDOW_H);
        setx(menuwindow[i],i*BUTTON_W+(i+1)*MARGIN_X);
        sety(menuwindow[i],BUTTON_H+MARGIN_Y);
        cengine::environment[EENVIRONMENT_OVER].add(menuwindow[i],layer);
        menuwindow[i]->visibility(false);
    }  
}


void ceditor::load_fnc(cbutton *bttn, const char *identify) {
    //METTO L'OGGETTO NELLA LISTA DI OGGETTI
    ctarga* context=NULL;
    if( canimation::loadmodel(&context, "torre.tga")){
//        elog("Loading item ok!");
        citem* itm=enew citem();
        itm->context=(ccontext*)context;
        citemhandler::add(itm, 0);
    }
//    else elog ("Loading error ... check filepath");
}
void ceditor::unload_fnc(cbutton *bttn, const char *identify) {
        citemhandler::del(citemhandler::itemlist[0]->nextvalue());
}

void ceditor::drawcomponent(char* id, int i) {
    sxmlclass *xmlblock[2] = {NULL, NULL};
    if (xmlblock[0] = editor->get("ss", "editor", "menu")) {
        if (strcasecmp(xmlblock[0]->keys.get("ID"),id)==0) {
            foreach(xmlblock[1], &xmlblock[0]->subclass) {
                
                int index = atoi(xmlblock[1]->keys.get("INDEX"));
                if (strcmp(xmlblock[1]->label, "button")==0) {

//                    TODO: SISTEMARE PUNTATORI
//                    if (clabel *lbl = enew clabel()) {
//                        lbl->initialize(&cengine::theme);
//                        lbl->content(xmlblock[1]->value);
//                        setx(lbl,-4);
//                        if (cbutton *btn = enew cbutton(lbl)) {
//                            btn->initialize(&cengine::theme);
//                            btn->handleizer(xmlblock[1]->value, ceditor::unload_fnc);
//                            setw(btn,BUTTON_W);
//                            seth(btn,BUTTON_H);
//                            setx(btn,MARGIN_X);
//                            sety(btn,index*(BUTTON_H+MARGIN_Y));
//                            menuwindow[i]->add(btn,xmlblock[1]->value);
//                        }
//                    }
                    
                } else if (strcmp(xmlblock[1]->label, "label")==0) {
                    
//                    TODO: SISTEMARE PUNTATORI
//                    if (clabel *tmp = enew clabel()) {
//                        tmp->initialize(&cengine::theme);
//                        tmp->content(xmlblock[1]->value);
//                        setw(tmp,BUTTON_W);
//                        seth(tmp,BUTTON_H);
//                        setx(tmp,MARGIN_X);
//                        sety(tmp,index*(BUTTON_H+MARGIN_Y));
//                        menuwindow[i]->add(tmp,xmlblock[1]->value);
//                        tmp->unload();
//                        delete(tmp);
//                    }
                    
                }
            }
        }
    }
}

void ceditor::forward (skeyboard *keyboard, smouse *mouse, unsigned int positionx, unsigned int positiony) {
    
    for (int i = 0; i < NUM_BUTTON; i++) {
        menubutton[i]->forward(NULL,mouse,positionx,positiony);
    }
    
}

void ceditor::checkselection (unsigned int positionx, unsigned int positiony, int layer) {
        citem* token=NULL;
        unsigned int delta = 2; 
        foreach(token, citemhandler::itemlist[layer] ){
            int posx = getx(token->context);
            int width = getw(token->context);
            int posy = gety(token->context);
            int height = geth(token->context);
            float mask = 1.0f;
//            elog("%d %d",getx(token->context),gety(token->context));
         
            if (positionx>posx-delta && positionx<posx+width+delta){
                if (positiony>posy-delta && positiony<posy+height+delta){
                    token->selected = !token->selected;
                    if (token->selected) mask = 0.0f;
                    token->context->masking(ECOLOR_R,mask);
                }
            }

        }
    
}

void ceditor::dragselection (unsigned int curx, unsigned int cury, unsigned int precx, unsigned int precy, unsigned int layer) {
        citem* token=NULL;
        foreach(token, citemhandler::itemlist[layer] ){
            if (token->selected) {
                int deltax = curx - precx;
                int deltay = cury - precy;
                elog("%d %d %d %d",curx, cury, precx, precy);
                setx(token->context,getx(token->context)+deltax);
                sety(token->context,gety(token->context)+deltay);
            }
        }
}

int ceditor::conclusion() {
    for (int i = 0; i < NUM_BUTTON; i++) {
        if (menulabel[i]) {
            menulabel[i]->unload();
            delete menulabel[i];
        }
        if (menubutton[i]) {
            menubutton[i]->unload();
            delete menubutton[i];
        }
        if (menuwindow[i]) {
            menuwindow[i]->unload();
            delete menuwindow[i];
        }
    }  
    
    //CANCELLAZIONE LISTE STATICHE
    citemhandler::unload();
    delete citemhandler::itemlist;
    
    editor->unload();
    delete(editor); 
    return EE_OK;
}
