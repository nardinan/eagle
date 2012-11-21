/* 
 * File:   citem.h
 * Author: 3570K
 *
 * Created on 8 novembre 2012, 11.15
 */

#ifndef CITEM_H
#define	CITEM_H
#include "cengine.h"

enum econtexttype{
    CONTEXT_TARGA=0,
    CONTEXT_MODEL,
    CONTEXT_ABSTRACT,
    CONTEXT_NULL
};
class citem: public eagleclass {
public:
    citem();
    inline void init(){
    }
    inline void unload(){
    this->context->unload();
    delete this->context;
    }
    public:
    char label[EE_LABEL_SIZE];
    int layer=0;
    bool selected=false;
    econtexttype type;
public:
    ccontext* context=NULL;
};

#endif	/* CITEM_H */

