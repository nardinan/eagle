/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "clights.h"
void clights::init (void) {
    lights.init();
    darkness = light = NULL;
    framebuffer = EE_NULLTEXTURE;
    shadows.init();
}

int clights::initialize (unsigned int resolutionx, unsigned int resolutiony) {
    scolor background = {EE_MASK_BACK}, spotting = {EE_MASK_SPOT};
    int result = EE_OK;
    GLuint texture;
    base = 1.0;
    if ((darkness = enew ctarga())) {
        if ((result = darkness->initialize(resolutionx, resolutiony, background)))
            if ((total = enew ctarga())) {
                if ((result = total->initialize(resolutionx, resolutiony, background)))
                    if ((light = enew ctarga())) {
                        if ((result = light->initialize(EE_LIGHT_SIZE, EE_LIGHT_SIZE, spotting)))
                            if ((result = light->invisibility(255, (EE_LIGHT_SIZE/2), (EE_LIGHT_SIZE/2), (EE_LIGHT_SIZE/2)))) {
                                singletexture(texture, total);
                                glGenFramebuffers(1, &framebuffer);
                                glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
                                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, EE_EXTENSION, texture, 0);
                                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                            }
                    } else
                        ekill("out of memory");
            } else
                ekill("out of memory");
    } else
        ekill("out of memory");
    return result;
}

int clights::add (int positionx, int positiony, const char *label) {
    slight *spot;
    int result = EE_OK;
    if ((spot = (slight *) emalloc(sizeof(slight)))) {
        spot->positionx = positionx;
        spot->positiony = positiony;
        spot->intensity = EE_LIGHT_INTENSITY;
        result = lights.add(spot, label);
    } else
        ekill("out of memory");
    return result;
}

void clights::del (const char *label) {
    slight *spot;
    if ((spot = lights.get(label))) {
        efree(spot);
        lights.del();
    }
}

void clights::redisplay (int view[], float zoom[], unsigned int resolution[]) {
    slight *spot;
    sshadow *shadow;
    ssilhouette *silhouette;
    float coordinates[EPOSITION_NULL];
    GLuint texture;
    spoint source, destination;
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    while ((texture = darkness->get()) != EE_NULLTEXTURE) {
        coordinates[EPOSITION_POSITIONX] = coordinates[EPOSITION_POSITIONY] = 0;
        coordinates[EPOSITION_WIDTH] = resolution[EPOSITION_WIDTH];
        coordinates[EPOSITION_HEIGHT] = resolution[EPOSITION_HEIGHT];
        erender(darkness,texture,coordinates);
    }
    foreach(spot, &lights) {
        glBlendFunc(GL_ZERO, GL_SRC_ALPHA);
        while ((texture = light->get()) != EE_NULLTEXTURE) {
            glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
            coordinates[EPOSITION_WIDTH] = coordinates[EPOSITION_HEIGHT] = spot->intensity;
            coordinates[EPOSITION_POSITIONX] = (spot->positionx-(coordinates[EPOSITION_WIDTH]/2.0));
            coordinates[EPOSITION_POSITIONY] = (spot->positiony-(coordinates[EPOSITION_HEIGHT]/2.0));
            enormalize(view,zoom,resolution,coordinates);
            erender(light,texture,coordinates);
        }
        source.positionx = spot->positionx;
        source.positiony = spot->positiony;
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        foreach(shadow, &shadows.shadows) {
            destination.positionx = shadow->context->destination(EPOSITION_POSITIONX)+(shadow->context->destination(EPOSITION_WIDTH)/2.0f);
            destination.positiony = shadow->context->destination(EPOSITION_POSITIONY)+(shadow->context->destination(EPOSITION_HEIGHT)/2.0f);
            if ((ccollision::distance(source, destination) < spot->intensity)) {
                cshadows::recalculate(shadow, source, spot->intensity, base);
                foreach(silhouette, &shadow->shape) {
                    glBegin(GL_QUADS);
                    {
                        glColor4f(0.0f, 0.0f, 0.0f, silhouette->alpha[0]);
                        coordinates[EPOSITION_POSITIONX] = silhouette->source.positionx;
                        coordinates[EPOSITION_POSITIONY] = silhouette->source.positiony;
                        enormalizesrc(view, zoom, resolution, coordinates);
                        glVertex3f(coordinates[EPOSITION_POSITIONX], coordinates[EPOSITION_POSITIONY], 0.0f);
                        glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
                        coordinates[EPOSITION_POSITIONX] = silhouette->projection[0].positionx;
                        coordinates[EPOSITION_POSITIONY] = silhouette->projection[0].positiony;
                        enormalizesrc(view, zoom, resolution, coordinates);
                        glVertex3f(coordinates[EPOSITION_POSITIONX], coordinates[EPOSITION_POSITIONY], 0.0f);
                        coordinates[EPOSITION_POSITIONX] = silhouette->projection[1].positionx;
                        coordinates[EPOSITION_POSITIONY] = silhouette->projection[1].positiony;
                        enormalizesrc(view, zoom, resolution, coordinates);
                        glVertex3f(coordinates[EPOSITION_POSITIONX], coordinates[EPOSITION_POSITIONY], 0.0f);
                        glColor4f(0.0f, 0.0f, 0.0f, silhouette->alpha[1]);
                        coordinates[EPOSITION_POSITIONX] = silhouette->destination.positionx;
                        coordinates[EPOSITION_POSITIONY] = silhouette->destination.positiony;
                        enormalizesrc(view, zoom, resolution, coordinates);
                        glVertex3f(coordinates[EPOSITION_POSITIONX], coordinates[EPOSITION_POSITIONY], 0.0f);
                    }
                    glEnd();
                }
            }
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    while ((texture = total->get()) != EE_NULLTEXTURE) {
        coordinates[EPOSITION_POSITIONX] = coordinates[EPOSITION_POSITIONY] = 0;
        coordinates[EPOSITION_WIDTH] = resolution[EPOSITION_WIDTH];
        coordinates[EPOSITION_HEIGHT] = resolution[EPOSITION_HEIGHT];
        glColor4f(0.0f, 0.0f, 0.0f, base);
        erender(total,texture,coordinates);
    }
}

int clights::listening (cxml &parameters) {
    sxmlclass *xmlsingleton[2] = {NULL, NULL};
    slight *spot;
    char *value, label[EE_LABEL_SIZE];
    int result = EE_OK;
    /* 
        <lights>
            <light ID=ID positionx=150 positiony=150 intensity=150 />
            [...]
        </lights>
     */
    if ((xmlsingleton[0] = parameters.get("ss", "engine", "lights"))) {
        this->base = ((value = xmlsingleton[0]->keys.get("base"))?((float)eatoi(value)/255.0f):EE_MASK_BASE);
        foreach(xmlsingleton[1], &xmlsingleton[0]->subclass) {
            if (estrcmp(xmlsingleton[1]->label, "light") == 0) {
                if ((spot = (slight *) emalloc(sizeof(slight)))) {
                    strand(label, EE_LABEL_SIZE);
                    spot->positionx = ((value = xmlsingleton[1]->keys.get("positionx"))?eatoi(value):0);
                    spot->positiony = ((value = xmlsingleton[1]->keys.get("positiony"))?eatoi(value):0);
                    spot->intensity = ((value = xmlsingleton[1]->keys.get("intensity"))?eatoi(value):EE_LIGHT_INTENSITY);
                    if (!(result = lights.add(spot, ((value = xmlsingleton[1]->keys.get("ID"))?value:label))))
                        break;
                } else
                    ekill("out of memory");
            }
        }
    }
    return result;
}

void clights::unload (sshadow *singleton) {
    destroy(&singleton->convexhull, efree, spoint);
    destroy(&singleton->shape, efree, ssilhouette);
    efree(singleton);
}

void clights::unload (void) {
    destroy(&lights, efree, slight);
    if (light) {
        light->unload();
        delete(light);
    }
    if (darkness) {
        darkness->unload();
        delete(darkness);
    }
    if (total) {
        total->unload();
        delete(total);
    }
    if (framebuffer != EE_NULLTEXTURE)
        glDeleteFramebuffers(1, &framebuffer);
    shadows.unload();
    this->init();
}