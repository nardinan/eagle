/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Filippo Paolucci
 * EMAIL        : fantic86@alice.it
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "canimation.h"
int canimation::fileexist(const char* filename) {
    FILE* fs = fopen(filename, "rb");
    int result = EE_OK;
    if (fs) {
        fclose(fs);
        if (EE_CANIMATION_PRINT)
            elog("filename: %s file founded", filename);
    } else {
        result = EE_ERROR;
        elog("filename: %s file doesn't exist", filename);
    }
    return result;
}

int canimation::loadmodel(cmodel** modelptr, const char* packagefilename) {
    cfilesystem* fs;
    cmodel* model = *modelptr;
    int result = EE_OK;
    if (model)
        elog("modelptr: model costructed yet or pointer not initialized to NULL");
    if (fileexist(packagefilename)) {
        if ((fs = enew cfilesystem())) {
            if ((result = fs->add(packagefilename, "modelfs"))) {
                if ((model = enew cmodel())) {
                    if ((result = model->initialize(fs->get("modelfs")))) {
                        if (EE_CANIMATION_PRINT)
                            elog("model: %s initialized correctly", packagefilename);
                        *modelptr = model;
                        fs->unload();
                        delete(fs);
                    } else
                        elog("model: impossible to initialize the model");
                } else
                    ekill("out of memory");
            }
            if (!result) {
                fs->unload();
                delete(fs);
            }
        } else
            ekill("out of memory");
    } else
        result = EE_ERROR;
    return result;
}

int canimation::loadmodel(cfx** modelptr, const char* packagefilename) {
    cfilesystem* fs;
    cfx* model = *modelptr;
    int result = EE_OK;
    if (model)
        elog("modelptr: model builded yet or pointer not initialized to NULL");
    if (fileexist(packagefilename)) {
        if ((fs = enew cfilesystem())) {
            if ((result = fs->add(packagefilename, "modelfs"))) {
                if ((model = enew cfx())) {
                    if ((result = model->initialize(fs->get("modelfs")))) {
                        if (EE_CANIMATION_PRINT)
                            elog("model: %s initialized correctly", packagefilename);
                        *modelptr = model;
                        fs->unload();
                        delete(fs);
                    } else
                        elog("model: impossible to initialize the model");
                } else
                    ekill("out of memory");
            } else
                elog("model: impossible to allocate a new model");
            if (!result) {
                fs->unload();
                delete(fs);
            }
        } else
            ekill("out of memory");
    } else
        result = EE_ERROR;
    return result;
}

int canimation::loadmodel(ctarga** modelptr, const char* packagefilename) {
    cfilesystem* fs = NULL;
    ctarga* model = *modelptr;
    int result = EE_OK;
    if (model) {
        elog("modelptr: model costructed yet or pointer not initialized to NULL");
        model = NULL;
    }
    if (fileexist(packagefilename)) {
        fs = enew cfilesystem();
        if (fs->add(packagefilename, "modelfs")) {
            if ((model = enew ctarga())) {
                if (model->initialize(fs->get("modelfs"))) {
                    if (EE_CANIMATION_PRINT)
                        elog("model: %s initialized correctly", packagefilename);
                    *modelptr = model;
                    fs->unload();
                    delete(fs);
                } else {
                    elog("model: impossible to initialize the model");
                    result = EE_ERROR;
                }
            } else {
                elog("model: impossible to allocate a new model");
                result = EE_ERROR;
            }
        } else
            result = EE_ERROR;
        fs->unload();
        delete(fs);
    } else
        result = EE_ERROR;
    return result;
}

int canimation::setmodel(cmodel* model, const char* movementlabel) {
    int result = EE_OK;
    if (model)
        result = model->set(movementlabel);
    else {
        result = EE_ERROR;
        elog("model: variable model not initialized");
    }
    return EE_ERROR;
}
