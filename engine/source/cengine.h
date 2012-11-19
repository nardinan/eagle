/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CENGINE_H
#define EE_CENGINE_H
#include "cground.h"
#include "cgraphics.h"
#include "caudio.h"
#define EE_W 640
#define EE_H 480
#define EE_FPS 60.0f
#define EE_R 0
#define EE_G 0
#define EE_B 0
#define EE_A 255
#define EE_USEC 1000000.0f
#define EE_TITLE "eagle engine version 1.5.3a"
#define EE_IMAGE "image.default"
#define EE_DEFAULT_AUDIO -1
#define EE_DEFAULT_FREQUENCY 44100
#define ptrzerostep(a,b,ch)\
    if ((a)=strchr((b),ch)){\
        *(a)='\0';\
        (a)++;\
    }
enum eenvironment { 
    EENVIRONMENT_UNDER = 0, 
    EENVIRONMENT_OVER,
    EENVIRONMENT_NULL
};
enum efps {
    EFPS_TODOFPS = 0,
    EFPS_DONEFPS,
    EFPS_UPDATE,
    EFPS_NULL
};
extern bool atob (const char *value);
extern const scolor white, black, red, yellow, green, cyan, blue, purple;
typedef int (fenginehandler)(void);
class cengine:public eagleclass {
public:
    static cinput inputs;
    static ctimer timers;
    static clobby lobby;
    static cfilesystem filesystem;
    static int screenview[EPOSITION_NULL];
private:
    static ctimer fps;
    static cfilesystem interfaces;
    static int view[EPOSITION_NULL];
    static float zoom[EPOSITION_NULL];
    static ALCdevice *device;
    static ALCcontext *context;
public:
    static cenvironment environment[EENVIRONMENT_NULL];
    static cchannels channels;
    static cfonts fonts;
    static ctheme theme;
    static clights lights;
    static cimpacts impacts;
    static cpaths paths;
    static float frames[EFPS_NULL];
private:
    static unsigned int resolution[EPOSITION_NULL], counter;
    static scolor background;
    static fenginehandler *loop, *quit;
    static cxml configuration;
public:
    static int initialize (const char *configuration, int *argc, char **argv, flobbyinstance *reference = NULL);
    static int initialize (const char *interface);
private:
    static void parameters (void);
public:
    static void viewport (eposition position, int value);
    static inline int viewport (eposition position) { return cengine::view[position]; };
private:
    static void graphichooks (void);
    static void audiohooks (void);
    static void inputhooks (void);
public:
    static inline void gameloop (fenginehandler *loop) { cengine::loop = loop; };
    static inline void gamequit (fenginehandler *quit) { cengine::quit = quit; };
    static inline int screen (eposition position) { return cengine::resolution[position]; };
    static void normalize (spoint points);
private:
    static void mouse (int button, int state, int positionx, int positiony);
    static inline void motion (int positionx, int positiony) { cengine::inputs.motion(positionx, positiony, cengine::resolution[EPOSITION_WIDTH], cengine::resolution[EPOSITION_HEIGHT]); };
    static inline void kup (unsigned char key, int positionx, int positiony) { cengine::inputs.analyze(EINPUT_KEYBOARD, EACTION_RELEASED, key, positionx, positiony); };
    static inline void sup (int key, int positionx, int positiony) { cengine::inputs.analyze(EINPUT_SPECIAL, EACTION_RELEASED, key, positionx, positiony); };
    static inline void kdown(unsigned char key, int positionx, int positiony) { cengine::inputs.analyze(EINPUT_KEYBOARD, EACTION_PRESSED, key, positionx, positiony); };
    static inline void sdown (int key, int positionx, int positiony) { cengine::inputs.analyze(EINPUT_SPECIAL, EACTION_PRESSED, key, positionx, positiony); };
    static void redisplay (void);
    static void reanimate (void);
    static void reshape (int width, int height);
public:
    static void sync (int usecs);
    static inline void start (void) { glutMainLoop(); };
    static void block (unsigned int code);
    static void unload (unsigned int code);
};
#endif
