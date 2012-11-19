#define EE_WAITING 3000000
#include "main.h"
pthread_mutex_t mutexes[EMUTEX_NULL];
ctarga *loading[2];
float alpha[2];
estatus status = ESTATUS_LOAD_ENTER;
int clean (void) {
    int result = EE_OK;
    /* unload code (freeing all resources) */
    return result;
}

void *building (void *parameters) {
    /* load game resources into filesystem */  
    cengine::sync(EE_WAITING);
    /* finished */
    safemethod(status = ESTATUS_LOAD_EXIT, mutexes[EMUTEX_STATUS]);
    pthread_exit(NULL);
    return NULL;
}

int loop (void) {
    pthread_t link;
    pthread_attr_t attributes;
    int result = EE_OK, index;
    skeyboard *key = cengine::inputs.key();
    if (key) {
        if (key->key == (char)27) /* esc key */
            result = EE_DONE;
        efree(key);
    }
    pthread_mutex_lock(&mutexes[EMUTEX_STATUS]);
    fflush(stdout);
    switch (status) {
        case ESTATUS_LOAD_ENTER:
            cengine::inputs.listening(EINPUT_KEYBOARD, false);
            cengine::inputs.listening(EINPUT_SPECIAL, false);
            cengine::inputs.listening(EINPUT_MOUSE, false);
            for (index = 0; index < EMUTEX_STATUS; index++)
                pthread_mutex_init(&mutexes[index], NULL);
            pthread_attr_init(&attributes);
            if (pthread_create(&link, &attributes, &building, NULL) == 0)
                pthread_detach(link);
            else
                result = EE_ERROR;
            status = ESTATUS_LOAD;
            break;
        case ESTATUS_LOAD:
            alpha[0] = EE_MIN((alpha[0]+0.01f), 1.0f);
            alpha[1] = EE_MIN((alpha[1]+0.01f), 1.0f);
            seta(loading[0], alpha[0]);
            seta(loading[1], alpha[1]);
            loading[0]->rotation += 0.25f;
            break;
        case ESTATUS_LOAD_EXIT:
            if ((alpha[0] > 0.0f) || (alpha[1] > 0.0f)) {
                alpha[0] = EE_MAX((alpha[0]-0.01f), 0.0f);
                alpha[1] = EE_MAX((alpha[1]-0.01f), 0.0f);
                seta(loading[0], alpha[0]);
                seta(loading[1], alpha[1]);
                loading[0]->rotation += 0.25f;
            } else {
                if ((cengine::environment[EENVIRONMENT_OVER].get(loading[0])))
                    cengine::environment[EENVIRONMENT_OVER].del();
                if ((cengine::environment[EENVIRONMENT_OVER].get(loading[1])))
                    cengine::environment[EENVIRONMENT_OVER].del();
                loading[0]->unload();
                delete(loading[0]);
                loading[1]->unload();
                delete(loading[1]);
                status = ESTATUS_MENU_ENTER;
            }
            break;
        case ESTATUS_MENU_ENTER:
            /* prepare menu interface */
            cengine::inputs.listening(EINPUT_KEYBOARD, true);
            cengine::inputs.listening(EINPUT_SPECIAL, true);
            cengine::inputs.listening(EINPUT_MOUSE, true);
            status = ESTATUS_MENU;
            break;
        case ESTATUS_MENU:
            /* menu management */
            break;
        case ESTATUS_MENU_EXIT:
            cengine::inputs.listening(EINPUT_KEYBOARD, false);
            cengine::inputs.listening(EINPUT_SPECIAL, false);
            cengine::inputs.listening(EINPUT_MOUSE, false);
            break;
        case ESTATUS_PLAY_ENTER:
        case ESTATUS_PLAY:
        case ESTATUS_PLAY_EXIT:
        case ESTATUS_PAUSE_ENTER:
        case ESTATUS_PAUSE:
        case ESTATUS_PAUSE_EXIT:
            break;
    }
    pthread_mutex_unlock(&mutexes[EMUTEX_STATUS]);
    return result;
}

int main (int argc, char *argv[]) {
    cfilesystem loader;
    int index;
    for (index = 0; index < EMUTEX_NULL; index++)
        pthread_mutex_init(&mutexes[index], NULL);
    if ((cengine::initialize("./default.config.xml", &argc, argv)) == EE_OK) {
        if ((cengine::initialize("./default.theme.dgt")) == EE_OK) {
            cengine::gameloop(loop);
            cengine::gamequit(clean);
            loader.init();
            if (loader.initialize("./default.loader.dfs")) {
                alpha[0] = alpha[1] = 0.0f;
                if ((loading[0] = enew ctarga())) {
                    if (loading[0]->initialize(loader.get("down"))) {
                        seta(loading[0], alpha[0]);
                        setx(loading[0], 10);
                        sety(loading[0], 400);
                        if ((loading[1] = enew ctarga())) {
                            if ((loading[1]->initialize(loader.get("up")))) {
                                seta(loading[1], alpha[1]);
                                setx(loading[1], 10);
                                sety(loading[1], 400);
                                cengine::environment[EENVIRONMENT_OVER].add(loading[0], 0);
                                cengine::environment[EENVIRONMENT_OVER].add(loading[1], 1);
                                loader.unload();
                                cengine::start();
                            }
                        } else
                            ekill("out of memory");
                    }
                } else
                    ekill("out of memory");
            }
        }
    }
    return 0;
}
