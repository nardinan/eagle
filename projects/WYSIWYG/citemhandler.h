/* 
 * File:   citemhandler.h
 * Author: 3570K
 *
 * Created on 8 novembre 2012, 17.01
 */

#ifndef CITEMHANDLER_H
#define	CITEMHANDLER_H
#include "cengine.h"
#include "citem.h"

#define EF_LAYERS 100

class citemhandler: public eagleclass {
public:
    
    static void init();
    static int conclusion();
    static int add(citem* itm, int layer);
    static int del(citem* itm);
    static int showlayer(bool visible, int layer);
    static void unload(citem* itm);
    static void unload();
    //static carray<carray<citem>*>* layerlist;
    static carray<citem>* itemlist[EF_LAYERS];

    static  int OFFSETX;
    static  int OFFSETY;
};
#endif	/* CITEMHANDLER_H */

