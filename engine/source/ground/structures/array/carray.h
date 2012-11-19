/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CARRAY_H
#define EE_CARRAY_H
#include "cmemory.h"
#define EE_LABEL_SIZE 128
#define EE_BUCKET_SIZE 8
#define EE_NO_ID -1
#define foreach(tok,arr)\
    (arr)->reset();\
    while (((tok) = (arr)->nextvalue()))
#define destroy(arr,fun,typ)\
    do {\
        typ *sin;\
        if (arr) {\
            foreach(sin,(arr))\
                fun((typ*)sin);\
            (arr)->unload();\
        }\
    } while(0)
#define estrlen(x)\
    ((x)?strlen(x):0)
#define estrcmp(x,y)\
    ((!x&&!y)?0:((!x)?1:((!y)?-1:strcmp(x,y))))
template <class edatum> class carray:public eagleclass {
private:
    struct srecord {
        edatum *datum;
        char label[EE_LABEL_SIZE];
    } **dataroot, **current;
    size_t elements, maximum, bucket;
public:
    carray (void) { 
        this->init(); 
    }
    
    carray (size_t bucket) { 
        this->init(bucket); 
    }
    
    void init (size_t bucket=EE_BUCKET_SIZE) {
        dataroot = current = NULL;
        elements = maximum = 0;
        this->bucket = bucket;
    }
    
    int add (edatum *datum, const char *label=NULL) {
        srecord *block = NULL;
        int result = EE_OK;
        if ((block = (srecord *) emalloc(sizeof(srecord)))) {
            memset(block->label, '\0', (sizeof(char)*EE_LABEL_SIZE));
            block->datum = datum;
            if (label)
                strncpy(block->label, label, (EE_LABEL_SIZE-1));
            if (maximum < (elements+1))
                if ((dataroot = (srecord **) erealloc(dataroot, ((maximum+bucket)*sizeof(srecord*))))) {
                    maximum += bucket;
                    current = NULL;
                } else
                    ekill("out of memory");
            memcpy((dataroot+(elements++)), (srecord *)(&block), (size_t)sizeof(srecord*));
        } else
            ekill("out of memory");
        return result;
    }
    
    void del (void) {
        srecord **backup = NULL;
        if (current) {
            efree(*current);
            backup = (current+1);
            if ((unsigned int)(backup-dataroot) < (elements--))
                memmove(current, backup, (((elements+1)-(backup-dataroot))*sizeof(srecord*)));
            current = NULL;
        }
    }
    
    edatum *get (size_t position) {
        current = NULL;
        if (position < elements)
            current = (srecord **)(dataroot+position);
        return (current)?(*current)->datum:NULL;
    }
    
    edatum *get (void) { 
        return (current)?(*current)->datum:NULL; 
    }
    
    edatum *get (const char *label) {
        edatum *result = NULL;
        if (estrcmp(this->label(), label) != 0) {
            foreach(result, this)
                if (estrcmp(this->label(), label) == 0)
                    break;
        } else
            result = this->get();
        return result;
    }
    
    edatum *search (edatum *value) {
        edatum *result = NULL;
        if ((result = this->get()) != value)
            foreach(result, this)
                if (result==value)
                    break;
        return result;
    }
    
    char *label (void) { 
        return (current)?(*current)->label:NULL; 
    }
    
    size_t index (void) { 
        return (current)?(size_t)(current-dataroot):EE_NO_ID; 
    }
    
    size_t records (void) { 
        return elements; 
    }
    
    size_t buckets (void) { 
        return maximum;
    }
    
    void reset (void) { 
        current = NULL; 
    }
private:
    void next (void) {
        srecord **backup = NULL;
        if (current) {
            backup = (current+1);
            if ((unsigned int)(backup-dataroot) < elements)
                current = backup;
            else
                current = NULL;
        } else
            current = (elements>0)?dataroot:NULL;
    }
public:
    edatum *nextvalue (void) {
        this->next();
        return (current)?(*current)->datum:NULL;
    }
    
    char *nextkey (void) {
        this->next();
        return (current)?(*current)->label:NULL;
    }
    
    void unload (void) {
        if (dataroot) {
            current = dataroot;
            while (((unsigned int)(current-dataroot) < elements)) {
                efree(*current);
                current++;
            }
            efree(dataroot);
            dataroot = NULL;
        }
        this->init();
    }    
};
#endif
