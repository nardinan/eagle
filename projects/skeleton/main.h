#include <cengine.h>
#define epoint(p,a,b)\
    do{\
        p.positionx=a;\
        p.positiony=b;\
    }while(0)
enum estatus {
    ESTATUS_LOAD_ENTER=0,
    ESTATUS_LOAD,
    ESTATUS_LOAD_EXIT,
    ESTATUS_MENU_ENTER,
    ESTATUS_MENU,
    ESTATUS_MENU_EXIT,
    ESTATUS_PLAY_ENTER,
    ESTATUS_PLAY,
    ESTATUS_PLAY_EXIT,
    ESTATUS_PAUSE_ENTER,
    ESTATUS_PAUSE,
    ESTATUS_PAUSE_EXIT
};
enum emutex {
    EMUTEX_STATUS=0,
    EMUTEX_NULL
};