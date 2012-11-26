/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "cengine.h"
bool atob (const char *value) {
    bool result = false;
    if (estrcmp(value, "true") == 0)
        result = true;
    return result;
}
const scolor white = {255, 255, 255, 255};
const scolor black = {0, 0, 0, 255};
const scolor red = {255, 0, 0, 255};
const scolor yellow = {255, 255, 0, 255};
const scolor green = {0, 255, 0, 255};
const scolor cyan = {0, 255, 255, 255};
const scolor blue = {0, 0, 255, 255};
const scolor purple = {255, 0, 255, 255};
cinput cengine::inputs;
ctimer cengine::timers, cengine::fps;
clobby cengine::lobby;
cfilesystem cengine::filesystem, cengine::interfaces;
cenvironment cengine::environment[EENVIRONMENT_NULL];
cchannels cengine::channels;
cfonts cengine::fonts;
ctheme cengine::theme;
clights cengine::lights;
cimpacts cengine::impacts;
cpaths cengine::paths;
int cengine::screenview[EPOSITION_NULL], cengine::view[EPOSITION_NULL];
float cengine::zoom[EPOSITION_NULL], cengine::frames[EFPS_NULL];
ALCdevice *cengine::device;
ALCcontext *cengine::context;
unsigned int cengine::counter, cengine::resolution[EPOSITION_NULL];
scolor cengine::background;
fenginehandler *cengine::loop, *cengine::quit;
cxml cengine::configuration;
int cengine::initialize (const char *configuration, int *argc, char **argv, flobbyinstance *reference) {
    sxmlclass *xmlsingleton;
    char glversion[EE_LABEL_SIZE], *pointer[3] = {NULL, NULL, NULL}, *title = NULL;
    int result = EE_OK;
    /*
    <engine>
        <resolutionx>800</resolutionx>
        <resolutiony>600</resolutiony>
        <fps>60</fps>
        <background>
            <r>0</r>
            <g>0</g>
            <b>0</b>
            <a>0</a>
        </background>
        <viewport>
            <x>0</x>
            <y>0</y>
            <w>800</w>
            <h>600</h>
        </viewport>
        <lobby enabled=true>
            [...]
        </lobby>
        <lights>
            [...]
        </lights>
        <channels>
            [...]
        </channels>
    </engine>
     */
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    if (cengine::filesystem.add(configuration, "engine.configuration")) {
        if (cengine::configuration.analyze(cengine::filesystem.get("engine.configuration"))) {
            cengine::parameters();
            glutInitWindowSize(cengine::resolution[EPOSITION_WIDTH], cengine::resolution[EPOSITION_HEIGHT]);
            if ((xmlsingleton = cengine::configuration.get("ss", "engine", "title")))
                title = xmlsingleton->value;
            glutCreateWindow((title)?title:EE_TITLE);
            glClearColor(cengine::background.red, cengine::background.green, cengine::background.blue, cengine::background.alpha);
            cengine::graphichooks();
            cengine::audiohooks();
            cengine::inputhooks();
            /* check version */
            memset(glversion, '\0', EE_LABEL_SIZE);
            snprintf(glversion, EE_LABEL_SIZE, "%s", glGetString(GL_VERSION));
            pointer[0] = glversion;
            ptrzerostep(pointer[1], pointer[0], '.');
            ptrzerostep(pointer[2], pointer[1], ' ');
            if (eatoi(pointer[0]) < 2)
                fprintf(stderr, "{WARNING} openGL drivers are dangerously outdated ((current)%s.%s < (required)2.0)\n", pointer[0], pointer[1]);
            /* end */
            if ((xmlsingleton = cengine::configuration.get("ss", "engine", "lobby")))
                if ((atob(xmlsingleton->keys.get("enabled"))) && (reference))
                    cengine::lobby.listening(cengine::configuration, reference);
            cengine::lights.init();
            cengine::lights.initialize(cengine::resolution[EPOSITION_WIDTH], cengine::resolution[EPOSITION_HEIGHT]);
            if ((xmlsingleton = cengine::configuration.get("ss", "engine", "lights")))
                cengine::lights.listening(cengine::configuration);
            if ((xmlsingleton = cengine::configuration.get("ss", "engine", "channels")))
                cengine::channels.listening(cengine::configuration);
            cengine::impacts.init();
            cengine::paths.init();
            cengine::fps.add("F");
            cengine::fps.add("C");
            cengine::counter = 0;
            esrand((unsigned int)time(NULL));
#ifdef _WIN32
            WORD wVersionRequested = MAKEWORD(1,0);
            WSADATA wsaData;
            WSAStartup(wVersionRequested, &wsaData);
#endif
        } else
            result = EE_ERROR;
        cengine::filesystem.del("engine.configuration");
    } else
        result = EE_ERROR;
    return result;
}

int cengine::initialize (const char *interface) {
    int result = EE_OK;
    if ((result = cengine::interfaces.initialize(interface))) {
        cengine::theme.init();
        if ((result = cengine::theme.initialize(&cengine::interfaces))) {
            ctarga::replacement = cengine::interfaces.get(EE_IMAGE); /* segment */
            cfonts::replacement = cengine::theme.get(ETHELAYOUT_FONT); /* context */
            clabel::replacement = cengine::theme.get(); /* cfonts */
        }
    } else
        result = EE_ERROR;
    return result;
}

void cengine::parameters (void) {
    sxmlclass *xmlsingleton;
    cengine::background.red = ((xmlsingleton = cengine::configuration.get("sss", "engine", "background", "r"))?eatoi(xmlsingleton->value):EE_R);
    cengine::background.green = ((xmlsingleton = cengine::configuration.get("sss", "engine", "background", "g"))?eatoi(xmlsingleton->value):EE_B);
    cengine::background.blue = ((xmlsingleton = cengine::configuration.get("sss", "engine", "background", "b"))?eatoi(xmlsingleton->value):EE_G);
    cengine::background.alpha = ((xmlsingleton = cengine::configuration.get("sss", "engine", "background", "a"))?eatoi(xmlsingleton->value):EE_A);
    cengine::resolution[EPOSITION_WIDTH] = ((xmlsingleton = cengine::configuration.get("ss", "engine", "resolutionx"))?eatoi(xmlsingleton->value):EE_W);
    cengine::resolution[EPOSITION_HEIGHT] = ((xmlsingleton = cengine::configuration.get("ss", "engine", "resolutiony"))?eatoi(xmlsingleton->value):EE_H);
    cengine::screenview[EPOSITION_WIDTH] = cengine::resolution[EPOSITION_WIDTH];
    cengine::screenview[EPOSITION_HEIGHT] = cengine::resolution[EPOSITION_HEIGHT];
    cengine::viewport(EPOSITION_POSITIONX, (xmlsingleton = cengine::configuration.get("sss", "engine", "viewport", "x"))?eatoi(xmlsingleton->value):0);
    cengine::viewport(EPOSITION_POSITIONY, (xmlsingleton = cengine::configuration.get("sss", "engine", "viewport", "y"))?eatoi(xmlsingleton->value):0);
    cengine::viewport(EPOSITION_WIDTH, (xmlsingleton = cengine::configuration.get("sss", "engine", "viewport", "w"))?eatoi(xmlsingleton->value):cengine::resolution[EPOSITION_WIDTH]);
    cengine::viewport(EPOSITION_HEIGHT, (xmlsingleton = cengine::configuration.get("sss", "engine", "viewport", "h"))?eatoi(xmlsingleton->value):cengine::resolution[EPOSITION_HEIGHT]);
    cengine::frames[EFPS_TODOFPS] = ((xmlsingleton = cengine::configuration.get("ss", "engine", "F"))?eatof(xmlsingleton->value):EE_FPS);
    cengine::frames[EFPS_UPDATE] = (EE_USEC/cengine::frames[EFPS_TODOFPS]);
}

void cengine::viewport (eposition position, int value) {
    cengine::view[position] = value;
    cengine::zoom[EPOSITION_WIDTH] = ((float)cengine::resolution[EPOSITION_WIDTH]/(float)EE_MAX(cengine::view[EPOSITION_WIDTH], 1.0f));
    cengine::zoom[EPOSITION_HEIGHT] = ((float)cengine::resolution[EPOSITION_HEIGHT]/(float)EE_MAX(cengine::view[EPOSITION_HEIGHT], 1.0f));
}

void cengine::graphichooks (void) {
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(EE_EXTENSION);
    glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glPointSize(1.0f);
	glLineWidth(1.0f);
    glutDisplayFunc(cengine::redisplay);
    glutReshapeFunc(cengine::reshape);
}

void cengine::audiohooks (void) {
    cengine::channels.init();
    if ((device = alcOpenDevice(NULL)))
        if ((context = alcCreateContext(device, NULL))) {
            alcMakeContextCurrent(context);
            alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
            alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
        }
}

void cengine::inputhooks (void) {
    cengine::inputs.reset();
    glutMouseFunc(cengine::mouse);
    glutPassiveMotionFunc(cengine::motion);
    glutMotionFunc(cengine::motion);
    glutKeyboardFunc(cengine::kdown);
    glutSpecialFunc(cengine::sdown);
    glutKeyboardUpFunc(cengine::kup);
    glutSpecialUpFunc(cengine::sup);
}

void cengine::normalize (spoint points) {
    points.positionx -= view[EPOSITION_POSITIONX];
    points.positiony -= view[EPOSITION_POSITIONY];
    points.positionx = ((float)points.positionx*zoom[EPOSITION_WIDTH]);
    points.positiony = ((float)points.positiony*zoom[EPOSITION_HEIGHT]);
}

void cengine::mouse (int button, int state, int positionx, int positiony) {
    switch (state) {
        case GLUT_UP:
            cengine::inputs.analyze(EINPUT_MOUSE, EACTION_RELEASED, button, positionx, positiony);
            break;
        case GLUT_DOWN:
            cengine::inputs.analyze(EINPUT_MOUSE, EACTION_PRESSED, button, positionx, positiony);
            break;
    }
}

void cengine::redisplay (void) {
	ccontext *singleton = NULL;
    GLuint texture = EE_NULLTEXTURE;
    float coordinates[EPOSITION_NULL], subcoordinates[EPOSITION_NULL], elpased;
    cengine::sync(cengine::frames[EFPS_UPDATE]);
    cengine::fps.set("C");
    cengine::frames[EFPS_UPDATE] = (EE_USEC/cengine::frames[EFPS_TODOFPS]);
    cengine::reanimate();
    glEnable(GL_BLEND);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    foreach(singleton, &cengine::environment[EENVIRONMENT_UNDER]) {
        while ((texture = singleton->get()) != EE_NULLTEXTURE) {
            coordinates[EPOSITION_POSITIONX] = singleton->destination(EPOSITION_POSITIONX, EPOSITIONSUBJECT_FRAME);
            coordinates[EPOSITION_POSITIONY] = singleton->destination(EPOSITION_POSITIONY, EPOSITIONSUBJECT_FRAME);
            coordinates[EPOSITION_WIDTH] = singleton->destination(EPOSITION_WIDTH, EPOSITIONSUBJECT_FRAME);
            coordinates[EPOSITION_HEIGHT] = singleton->destination(EPOSITION_HEIGHT, EPOSITIONSUBJECT_FRAME);
            if (singleton->viewported())
                enormalize(cengine::view,cengine::zoom,cengine::resolution,coordinates);
            if (singleton->subpixel) {
                subcoordinates[EPOSITION_POSITIONX] = singleton->destination(EPOSITION_POSITIONX, EPOSITIONSUBJECT_BEFORE);
                subcoordinates[EPOSITION_POSITIONY] = singleton->destination(EPOSITION_POSITIONY, EPOSITIONSUBJECT_BEFORE);
                subcoordinates[EPOSITION_WIDTH] = singleton->destination(EPOSITION_WIDTH, EPOSITIONSUBJECT_BEFORE);
                subcoordinates[EPOSITION_HEIGHT] = singleton->destination(EPOSITION_HEIGHT, EPOSITIONSUBJECT_BEFORE);
                if (singleton->viewported())
                    enormalize(cengine::view,cengine::zoom,cengine::resolution,coordinates);
                glColor4f(singleton->gloss(ECOLOR_R, EPOSITIONSUBJECT_FRAME), singleton->gloss(ECOLOR_G, EPOSITIONSUBJECT_FRAME), singleton->gloss(ECOLOR_B, EPOSITIONSUBJECT_FRAME), singleton->gloss(ECOLOR_A, EPOSITIONSUBJECT_FRAME)/2.0);
                erender(singleton,texture,subcoordinates);
            }
            glColor4f(singleton->gloss(ECOLOR_R, EPOSITIONSUBJECT_FRAME), singleton->gloss(ECOLOR_G, EPOSITIONSUBJECT_FRAME), singleton->gloss(ECOLOR_B, EPOSITIONSUBJECT_FRAME), singleton->gloss(ECOLOR_A, EPOSITIONSUBJECT_FRAME));
            erender(singleton,texture,coordinates);
            
        }
    }
    /* end */
    cengine::lights.redisplay(view, zoom, resolution);
    /* over light system */
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    foreach(singleton, &cengine::environment[EENVIRONMENT_OVER]) {
        while ((texture = singleton->get()) != EE_NULLTEXTURE) {
            coordinates[EPOSITION_POSITIONX] = singleton->destination(EPOSITION_POSITIONX, EPOSITIONSUBJECT_FRAME);
            coordinates[EPOSITION_POSITIONY] = singleton->destination(EPOSITION_POSITIONY, EPOSITIONSUBJECT_FRAME);
            coordinates[EPOSITION_WIDTH] = singleton->destination(EPOSITION_WIDTH, EPOSITIONSUBJECT_FRAME);
            coordinates[EPOSITION_HEIGHT] = singleton->destination(EPOSITION_HEIGHT, EPOSITIONSUBJECT_FRAME);
            if (singleton->viewported())
                enormalize(cengine::view,cengine::zoom,cengine::resolution,coordinates);
            if (singleton->subpixel) {
                subcoordinates[EPOSITION_POSITIONX] = singleton->destination(EPOSITION_POSITIONX, EPOSITIONSUBJECT_BEFORE);
                subcoordinates[EPOSITION_POSITIONY] = singleton->destination(EPOSITION_POSITIONY, EPOSITIONSUBJECT_BEFORE);
                subcoordinates[EPOSITION_WIDTH] = singleton->destination(EPOSITION_WIDTH, EPOSITIONSUBJECT_BEFORE);
                subcoordinates[EPOSITION_HEIGHT] = singleton->destination(EPOSITION_HEIGHT, EPOSITIONSUBJECT_BEFORE);
                if (singleton->viewported())
                    enormalize(cengine::view,cengine::zoom,cengine::resolution,subcoordinates);
                glColor4f(singleton->gloss(ECOLOR_R, EPOSITIONSUBJECT_FRAME), singleton->gloss(ECOLOR_G, EPOSITIONSUBJECT_FRAME), singleton->gloss(ECOLOR_B, EPOSITIONSUBJECT_FRAME), singleton->gloss(ECOLOR_A, EPOSITIONSUBJECT_FRAME)/2.0);
                erender(singleton,texture,subcoordinates);
            }
            glColor4f(singleton->gloss(ECOLOR_R, EPOSITIONSUBJECT_FRAME), singleton->gloss(ECOLOR_G, EPOSITIONSUBJECT_FRAME), singleton->gloss(ECOLOR_B, EPOSITIONSUBJECT_FRAME), singleton->gloss(ECOLOR_A, EPOSITIONSUBJECT_FRAME));
            erender(singleton,texture,coordinates);
        }
    }
    glDisable(GL_BLEND);
    glutSwapBuffers();
    cengine::counter++;
    if (cengine::fps.get("F").secs >= 1) {
        cengine::frames[EFPS_DONEFPS] = (((float)cengine::counter*1000000.0f)/(float)cengine::fps.get("F").usecs);
        cengine::fps.set("F");
        cengine::counter = 0;
    }
    if ((elpased = ((cengine::fps.get("C").usecs*cengine::frames[EFPS_TODOFPS]))) < 1000000.0f)
        cengine::frames[EFPS_UPDATE] = ((1000000.0f-elpased)/cengine::frames[EFPS_TODOFPS]);
    else
        cengine::frames[EFPS_UPDATE] = 0.0f;
}

void cengine::reanimate (void) {
    int errcode, result = EE_OK;
    cengine::impacts.recalculate();
    cengine::paths.recalculate();
    if (cengine::loop) {
        result = cengine::loop();
        if (result != EE_OK)
            cengine::block(result);
    }
    glutPostRedisplay();
    if ((errcode = glGetError()) != GL_NO_ERROR)
        fprintf(stderr, "openGL errorcode: %d\n", errcode);
}

void cengine::reshape (int width, int height) {
    cengine::screenview[EPOSITION_WIDTH] = width;
    cengine::screenview[EPOSITION_HEIGHT] = height;
    if ((cengine::resolution[EPOSITION_WIDTH] != width) || (cengine::resolution[EPOSITION_HEIGHT] != height))
        glutReshapeWindow(cengine::resolution[EPOSITION_WIDTH], cengine::resolution[EPOSITION_HEIGHT]);
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glViewport(0, 0, cengine::resolution[EPOSITION_WIDTH], cengine::resolution[EPOSITION_HEIGHT]); /* keeping window size */
	glOrtho(0.0f, cengine::resolution[EPOSITION_WIDTH], cengine::resolution[EPOSITION_HEIGHT], 0.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void cengine::sync (int usecs) {
    struct timeval timeout;
    fd_set dummy;
    int sck = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    FD_ZERO(&dummy);
    FD_SET(sck, &dummy);
    timeout.tv_sec = (usecs/1000000L);
    timeout.tv_usec = (usecs%1000000L);
    select(NULL, NULL, NULL, &dummy, &timeout);
    close(sck);
}

void cengine::block (unsigned int code) {
    if (cengine::quit)
        cengine::quit();
    cengine::unload(code);
}

void cengine::unload (unsigned int code) {
    int index;
    cengine::inputs.unload();
    cengine::timers.unload();
    cengine::fps.unload();
    cengine::filesystem.unload();
    cengine::interfaces.unload();
    for (index = 0; index < EENVIRONMENT_NULL; index++)
        cengine::environment[index].unload();
    cengine::fonts.unload();
    cengine::theme.unload();
    cengine::lights.unload();
    cengine::impacts.unload();
    cengine::paths.unload();
    cengine::configuration.unload();
    alcDestroyContext(context);
    alcCloseDevice(device);
    ememory(stdout);
    exit(code);
}
