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
    if (fs) {
        if (EE_CANIMATION_PRINT)
            elog("filename: %s file founded", filename);
        fclose(fs);
        return EE_OK;
    }
    elog("filename: %s file doesn't exist", filename);
    return EE_ERROR;
}

int canimation::loadmodel(cmodel** modelptr, const char* packagefilename) {
    cmodel* model = *modelptr;
    if (model) {
        //model->unload();
        //delete model;
        elog("modelptr: model costructed yet or pointer not initialized to NULL");
    }
    if (fileexist(packagefilename)) {
        cfilesystem* fs = enew cfilesystem();
        if (fs->add(packagefilename, "modelfs")) {
            if ((model = enew cmodel())) {
                if (model->initialize(fs->get("modelfs"))) {
                    if (EE_CANIMATION_PRINT)
                        elog("model: %s initialized correctly", packagefilename);
                    *modelptr = model;
                    fs->unload();
                    delete(fs);
                    return EE_OK;
                } else
                    elog("model: impossible to initialize the model");
            } else
                elog("model: impossible to allocate a new model");
        }
        fs->unload();
        delete(fs);
    }
    return EE_ERROR;
}
int canimation::loadmodel(ctarga** modelptr, const char* packagefilename) {
    ctarga* model = *modelptr;
    if (model) {
        //model->unload();
        //delete model;
        elog("modelptr: model costructed yet or pointer not initialized to NULL");
    }
    if (fileexist(packagefilename)) {
        cfilesystem* fs = enew cfilesystem();
        if (fs->add(packagefilename, "modelfs")) {
            if ((model = enew ctarga())) {
                if (model->initialize(fs->get("modelfs"))) {
                    if (EE_CANIMATION_PRINT)
                        elog("model: %s initialized correctly", packagefilename);
                    *modelptr = model;
                    fs->unload();
                    delete(fs);
                    return EE_OK;
                } else
                    elog("model: impossible to initialize the model");
            } else
                elog("model: impossible to allocate a new model");
        }
        fs->unload();
        delete(fs);
    }
    return EE_ERROR;
}
int canimation::loadmodel(cfx** modelptr, const char* packagefilename) {
    cfx* model = *modelptr;
    if (model)
        elog("modelptr: model builded yet or pointer not initialized to NULL");
    if (fileexist(packagefilename)) {
        cfilesystem* fs = enew cfilesystem();
        if (fs->add(packagefilename, "modelfs")) {
            if ((model = enew cfx())) {
                if (model->initialize(fs->get("modelfs"))) {
                    if (EE_CANIMATION_PRINT)
                        elog("model: %s initialized correctly", packagefilename);
                    *modelptr = model;
                    fs->unload();
                    delete(fs);
                    return EE_OK;
                } else
                    elog("model: impossible to initialize the model");
            } else
                elog("model: impossible to allocate a new model");
        }
        fs->unload();
        delete(fs);
    }
    return EE_ERROR;
}

int canimation::setmodel(cmodel* model, const char* movementlabel) {
    if (model)
        if (model->set(movementlabel))
            return EE_OK;
    elog("model: variable model not initialized");
    return EE_ERROR;
}