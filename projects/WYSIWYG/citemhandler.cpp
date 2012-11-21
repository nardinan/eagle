/* 
 * File:   citemhandler.cpp
 * Author: 3570K
 * 
 * Created on 8 novembre 2012, 17.01
 */

#include "citemhandler.h"

//carray<carray<citem>*>* citemhandler::layerlist;
carray<citem>*  citemhandler::itemlist [EF_LAYERS];
int citemhandler::OFFSETX; 
int citemhandler::OFFSETY;

void citemhandler::init() {
    citemhandler::OFFSETX=200;
    citemhandler::OFFSETY=200;
    //citemhandler::layerlist= enew carray<carray<citem>*> ();
        for(int i=0; i<EF_LAYERS;i++){
            char* buf=NULL;
            char num[16]="";
            itoa(i,num,16);
            strapp(buf,"itemlist");
            strapp(buf, num );
            citemhandler::itemlist[i]= enew carray<citem> ();
            //citemhandler::layerlist->add( &citemhandler::itemlist[i], buf);
            efree(buf);
        }
}
 
int citemhandler::add(citem* itm, int layer){
    if (layer>=0 && layer<EF_LAYERS)
        if (itm){
            strand(itm->label,32);
            cengine::environment[EENVIRONMENT_OVER].add( itm->context, layer, itm->label);
            setx(itm->context, citemhandler::OFFSETX+erand()%400);
            sety(itm->context, citemhandler::OFFSETY+erand()%300);
            citemhandler::itemlist[layer]->add(itm, itm->label);
            return EE_OK;
        }
    return EE_ERROR;
}
int citemhandler::del(citem* itm){
        if (itm){
            cengine::environment[EENVIRONMENT_OVER].get(itm->context);
            cengine::environment[EENVIRONMENT_OVER].del();
            citem* tempitm=citemhandler::itemlist[itm->layer]->get( itm->label );
            if (tempitm==itm) {
                citemhandler::itemlist[itm->layer]->del();
                itm->unload();
                delete itm;
                return EE_OK;
            }
//            else elog("Item label duplicated in list. Item is not unique augment labelsize for random generation");
        }
//        else elog ("Item not initialized or found.");
    return EE_ERROR;
}
int citemhandler::showlayer(bool visible, int layer){
    if (layer>=0 && layer<EF_LAYERS){
        citem* token=NULL;
        foreach(token, citemhandler::itemlist[layer] ){
//            elog("op %d", visible);
            float value=0.0f;
            if (visible){
                value=1.0f;
            } else {
                token->selected = false;
            }
            token->context->masking(ECOLOR_A,value);
        }
    }
    return EE_ERROR;
}

void citemhandler::unload(citem* itm) {
    itm->unload();
    delete itm;
}
void citemhandler::unload() {
        for(int i=0; i<EF_LAYERS;i++){
             destroy( citemhandler::itemlist[i] ,unload, citem);
             delete citemhandler::itemlist[i];
        }
}

