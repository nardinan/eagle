/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "cmodel.h"
void cmodel::init (void) {
    ccontext::init(); /* father's init */
    elements.init();
    actions.init();
    returned = false;
    hard = true;
}

int cmodel::initialize (sdfssegment *datablock) {
    cxml index;
    cfilesystem input;
    sxmlclass *xmlclass, *singleton;
    char *value;
    ctarga *context;
    int result = EE_OK;
    /*
     <animation>
        <set>
            <image ID=ID>image</image>
            <image ID=ID>image</image>
        </set>
        <actions>
            <action loop=-1 ID=ID>
                <!-- loop is awesome:
                    0: no loop
                    -1: infinite loop
                    1 .. MAX_INT: number of cycles
                -->
                <frame context=ID overx=10 overy=10 update=1500 />
                <frame context=ID overx=10 overy=10 update=1500 />
            </action>
        </actions>
     </animation>
     */
    if ((result = input.initialize(datablock))) {
        if ((result = index.analyze(input.get("index")))) {
            if ((xmlclass = index.get("ss", "animation", "set")))
                foreach(singleton, &xmlclass->subclass)
                    if (estrcmp(singleton->label, "image") == 0)
                        if ((value = singleton->keys.get("ID")))
                            if ((context = enew ctarga())) {
                                if ((result = context->initialize(input.get(singleton->value))))
                                    result = elements.add(context, value);
                            } else
                                ekill("out of memory");
            if ((xmlclass = index.get("ss", "animation", "actions")))
                foreach(singleton, &xmlclass->subclass)
                    if (estrcmp(singleton->label, "action") == 0)
                        if (!(result = action(singleton)))
                            break;
            index.unload();
        }
        input.unload();
    }
    return result;
}

int cmodel::initialize (cmodel *model, bool hard) {
    int result = EE_OK;
    size_t index[2];
    ctarga *image[2] = {NULL, NULL};
    saction *action[2] = {NULL, NULL};
    sframe *frame[2] = {NULL, NULL};
    this->hard = hard;
    drop(EPOSITION_WIDTH, model->destination(EPOSITION_WIDTH));
    drop(EPOSITION_HEIGHT, model->destination(EPOSITION_HEIGHT));
    rotation = model->rotation;
    flipped[EFLIPDIRECTION_HORIZONTAL] = model->flipped[EFLIPDIRECTION_HORIZONTAL];
    flipped[EFLIPDIRECTION_VERTICAL] = model->flipped[EFLIPDIRECTION_VERTICAL];
    masking(ECOLOR_R, model->gloss(ECOLOR_R));
    masking(ECOLOR_G, model->gloss(ECOLOR_G));
    masking(ECOLOR_B, model->gloss(ECOLOR_B));
    masking(ECOLOR_A, model->gloss(ECOLOR_A));
    foreach(image[0], &model->elements) {
        image[1] = image[0];
        if (this->hard) {
            if ((image[1] = enew ctarga())) {
                if (!(result = image[1]->initialize(image[0])))
                    break;
            } else
                ekill("out of memory");
        }
        if (result)
            result = elements.add(image[1], model->elements.label());
        if (!result)
            break;
    }
    if (result) {
        index[0] = model->actions.index();
        foreach(action[0], &model->actions) {
            action[1] = action[0];
            if (this->hard) {
                if ((action[1] = (saction *) emalloc(sizeof(saction)))) {
                    action[1]->loop = action[1]->current = action[0]->loop; /* reinitialize */
                    action[1]->frames.init();
                    action[1]->stopwatch.init();
                    if ((result = action[1]->stopwatch.add("d"))) {
                        snprintf(action[1]->label, EE_LABEL_SIZE, "%s", action[0]->label);
                        index[1] = action[0]->frames.index();
                        foreach(frame[0], &action[0]->frames) {
                            if ((frame[1] = (sframe *) emalloc(sizeof(sframe)))) {
                                *frame[1] = *frame[0];
                                if (!(result = action[1]->frames.add(frame[1])))
                                    break;
                            } else 
                                ekill("out of memory");
                        }
                        if (index[1] != EE_NO_ID)
                            action[0]->frames.get((int)index[1]);
                    }
                } else
                    ekill("out of memory");
            }
            if (result)
                result = actions.add(action[1], model->actions.label());
            if (!result)
                break;
        }
        if (index[0] != EE_NO_ID) {
            model->actions.get((int)index[0]);
            actions.get((int)index[0]);
        }
    }
    return result;
}

int cmodel::action (sxmlclass *xmlclass) {
    sxmlclass *xmlsingleton;
    saction *action;
    char *value;
    int result = EE_OK;
    if ((action = (saction *) emalloc(sizeof(saction)))) {
        action->loop = action->current = ((value = xmlclass->keys.get("loop")))?eatoi(value):EE_MODEL_INFINITE_LOOP;
        action->frames.init();
        action->stopwatch.init();
        if ((result = action->stopwatch.add("d"))) {
            if ((value = xmlclass->keys.get("context"))) {
                snprintf(action->label, EE_LABEL_SIZE, "%s", value);
                foreach(xmlsingleton, &xmlclass->subclass)
                    if (estrcmp(xmlsingleton->label, "frame") == 0)
                        if (!(result = frame(xmlsingleton, action)))
                            break;
                if (result)
                    result = actions.add(action, action->label);
            } else
                result = EE_ERROR;
        }
    } else
        ekill("out of memory");
    return result;
}

int cmodel::frame (sxmlclass *xmlclass, saction *action) {
    sframe *frame;
    char *value;
    int result = EE_OK;
    if ((frame = (sframe *) emalloc(sizeof(sframe)))) {
        if ((value = xmlclass->keys.get("context"))) {
            snprintf(frame->context, EE_LABEL_SIZE, "%s", value);
            frame->overlay[EPOSITION_POSITIONX] = ((value = xmlclass->keys.get("overx")))?eatoi(value):0;
            frame->overlay[EPOSITION_POSITIONY] = ((value = xmlclass->keys.get("overy")))?eatoi(value):0;
            frame->angle = ((value = xmlclass->keys.get("angle")))?atof(value):0.0;
            frame->update = ((value = xmlclass->keys.get("update")))?eatoi(value):EE_MODEL_UPDATE;
        } else
            result = EE_ERROR;
        if (result)
            result = action->frames.add(frame);
    } else
        ekill("out of memory");
    return result;
}

int cmodel::set (const char *label) {
    saction *action = actions.get(label);
    sframe *frame;
    ctarga *image;
    int result = EE_OK;
    if (action) {
        action->current = 0;
        action->stopwatch.set("d");
        drop(EPOSITION_WIDTH, 0);
        drop(EPOSITION_HEIGHT, 0);
        foreach(frame, &action->frames) {
            if ((image = elements.get(frame->context))) {
                if (destination(EPOSITION_WIDTH) < image->destination(EPOSITION_WIDTH))
                    drop(EPOSITION_WIDTH, image->destination(EPOSITION_WIDTH));
                if (destination(EPOSITION_HEIGHT) < image->destination(EPOSITION_HEIGHT))
                    drop(EPOSITION_HEIGHT, image->destination(EPOSITION_HEIGHT));
            }
        }
        action->frames.reset();
    } else
        result = EE_ERROR;
    return result;
}

int cmodel::invisibility (scolor color, unsigned int threshold, bool revealing) {
    ctarga *image;
    int result = EE_OK;
    foreach(image, &elements)
        if (!(result = image->invisibility(color, threshold, revealing)))
            break;
    return result;
}

scolor *cmodel::pixel (unsigned int positionx, unsigned int positiony) {
    scolor *result = NULL;
    saction *action;
    sframe *frame;
    ctarga *image;
    if ((action = actions.get()))
        if ((frame = action->frames.get()))
            if ((image = elements.get(frame->context)))
                result = image->pixel(positionx, positiony);
    return result;
}

GLuint cmodel::get (void) {
    GLuint result = EE_NULLTEXTURE;
    saction *action;
    sframe *frame;
    ctarga *image;
    if (!returned) {
        if ((action = actions.get())) {
            if (!(frame = action->frames.get())) {
                if ((action->loop == EE_MODEL_INFINITE_LOOP) || (action->current < action->loop)) {
                    if (action->loop != EE_MODEL_INFINITE_LOOP)
                        action->current++;
                    frame = action->frames.nextvalue(); /* resetting, first frame */
                }
            }
            if (frame) {
                if ((image = elements.get(frame->context))) {
                    singletexture(result, image);
                    coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_POSITIONX] = image->destination(EPOSITION_POSITIONX, EPOSITIONSUBJECT_FRAME)+destination(EPOSITION_POSITIONX);
                    coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_POSITIONY] = image->destination(EPOSITION_POSITIONY, EPOSITIONSUBJECT_FRAME)+destination(EPOSITION_POSITIONY);
                    coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_WIDTH] = image->destination(EPOSITION_WIDTH, EPOSITIONSUBJECT_FRAME);
                    coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_HEIGHT] = image->destination(EPOSITION_HEIGHT, EPOSITIONSUBJECT_FRAME);
                    coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_POSITIONX] = image->source(EPOSITION_POSITIONX, EPOSITIONSUBJECT_FRAME);
                    coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_POSITIONY] = image->source(EPOSITION_POSITIONY, EPOSITIONSUBJECT_FRAME);
                    coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_WIDTH] = image->source(EPOSITION_WIDTH, EPOSITIONSUBJECT_FRAME);
                    coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_HEIGHT] = image->source(EPOSITION_HEIGHT, EPOSITIONSUBJECT_FRAME);
                    channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_R] = mastercolor(image->gloss(ECOLOR_R),channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_R]);
                    channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_G] = mastercolor(image->gloss(ECOLOR_G),channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_G]);
                    channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_B] = mastercolor(image->gloss(ECOLOR_B),channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_B]);
                    channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_A] = mastercolor(image->gloss(ECOLOR_A),channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_A]);
                }
                if (action->stopwatch.get("d").usecs > frame->update) {
                    action->stopwatch.set("d");
                    action->frames.nextvalue(); /* ignoring value */
                }
            }
        }
    }
    returned = (!returned);
    return result;
}

void cmodel::restore (void) {
    ctarga *image;
    foreach(image, &elements)
    image->restore();
    channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_R] = channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_G] = channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_B] = channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_A] = 1.0f;
    channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_R] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_G] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_B] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_A] = 1.0f;
    rotation = 0.0f;
}

void cmodel::unload (saction *action) {
    action->stopwatch.unload();
    destroy(&action->frames, efree, sframe);
    efree(action);
}

void cmodel::unload (ctarga *targa) {
    targa->unload();
    delete(targa);
}

void cmodel::unload (void) {
    if (!hard) {
        actions.unload();
        elements.unload();
    } else {
        destroy(&actions, unload, saction);
        destroy(&elements, unload, ctarga);
    }
    this->init();
}
