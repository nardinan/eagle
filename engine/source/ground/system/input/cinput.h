/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CINPUT_H
#define EE_CINPUT_H
#include "carray.h"
#define EE_ASCII_SET (128+17)
#define EE_PENDINGS_SIZE 256
#define inrange(mus,x,y,w,h,res)\
    do{\
        res=false;\
        if(((mus)->positionx >= (x)) && ((mus)->positionx <= ((x)+(w))) && ((mus)->positiony >= (y)) && ((mus)->positiony <= ((y)+(h))))\
            res=true;\
    }while(0)
enum ekeyboard {
    EKEYBOARD_SPACE = 32,
    EKEYBOARD_NONE = 128, /* counting from 255 */
    EKEYBOARD_UP,
    EKEYBOARD_DOWN,
    EKEYBOARD_RIGHT,
    EKEYBOARD_LEFT,
    EKEYBOARD_F1,
    EKEYBOARD_F2,
    EKEYBOARD_F3,
    EKEYBOARD_F4,
    EKEYBOARD_F5,
    EKEYBOARD_F6,
    EKEYBOARD_F7,
    EKEYBOARD_F8,
    EKEYBOARD_F9,
    EKEYBOARD_F10,
    EKEYBOARD_F11,
    EKEYBOARD_F12
};
enum emouse {
    EMOUSE_LEFT,
    EMOUSE_RIGHT,
    EMOUSE_MIDDLE
};
enum einput {
    EINPUT_KEYBOARD,
    EINPUT_MOUSE,
    EINPUT_SPECIAL,
    EINPUT_NULL
};
enum eaction {
    EACTION_PRESSED,
    EACTION_RELEASED
};
typedef struct skeyboard {
    unsigned char key;
    eaction action;
    bool altmod, shiftmod, ctrlmod;
    int positionx, positiony;
} skeyboard;
typedef struct smouse {
    emouse key;
    eaction action;
    bool altmod, shiftmod, ctrlmod;
    int positionx, positiony;
} smouse;
extern bool ekeys[EE_ASCII_SET];
class cinput:public eagleclass {
private:
    carray<skeyboard> keyboard;
    carray<smouse> mouse;
    bool catcher[EINPUT_NULL];
    unsigned int pendings, mousex, mousey;
public:
    inline cinput (void) { this->init(EE_PENDINGS_SIZE); };
    inline cinput (unsigned int pendings) { this->init(pendings); };
    void init (unsigned int pendings=EE_PENDINGS_SIZE);
    void reset (void);
    void analyze (einput kind, eaction action, int key, int positionx, int positiony);
private:
    void press (einput kind, int key, int positionx, int positiony);
    void release (einput kind, int key, int positionx, int positiony);
public:
    void motion (int positionx, int positiony, int resolutionx, int resolutiony);
    inline void listening (einput kind, bool listen) { catcher[kind] = listen; };
    inline unsigned int positionx (void) { return mousex; };
    inline unsigned int positiony (void) { return mousey; };
    skeyboard *key (void);
    smouse *button (void);
    void unload (void);
};
#endif
