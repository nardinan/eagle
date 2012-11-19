/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "cparticle.h"
void cparticle::init (void) {
    ccontext::init(); /* father's init */
    master = NULL;
    particles.init();
    elements = EE_PARTICLE_ELEMENTS;
    position[EE_PARTICLE_FROM].overflow[EPOSITION_POSITIONX] = position[EE_PARTICLE_TO].overflow[EPOSITION_POSITIONX] = EE_PARTICLE_OVERFLOW_X;
    position[EE_PARTICLE_FROM].overflow[EPOSITION_POSITIONY] = position[EE_PARTICLE_TO].overflow[EPOSITION_POSITIONY] = EE_PARTICLE_OVERFLOW_Y;
    position[EE_PARTICLE_FROM].zoom[EPOSITION_WIDTH] = position[EE_PARTICLE_TO].zoom[EPOSITION_WIDTH] = EE_PARTICLE_ZOOM_W; 
    position[EE_PARTICLE_FROM].zoom[EPOSITION_HEIGHT] = position[EE_PARTICLE_TO].zoom[EPOSITION_HEIGHT] = EE_PARTICLE_ZOOM_H;
    life[EE_PARTICLE_FROM].regenerations = life[EE_PARTICLE_TO].regenerations = EE_PARTICLE_REGENERATIONS;
    life[EE_PARTICLE_FROM].ttl = life[EE_PARTICLE_TO].ttl = EE_PARTICLE_TTL;
    life[EE_PARTICLE_FROM].ttu[EPOSITION_POSITIONX] = life[EE_PARTICLE_FROM].ttu[EPOSITION_POSITIONY] = EE_PARTICLE_TTU;
    life[EE_PARTICLE_TO].ttu[EPOSITION_POSITIONX] = life[EE_PARTICLE_TO].ttu[EPOSITION_POSITIONY] = EE_PARTICLE_TTU;
    life[EE_PARTICLE_FROM].maxttu = life[EE_PARTICLE_TO].maxttu = EE_PARTICLE_TTU;
    forces[EE_PARTICLE_FROM].force.horizontal = forces[EE_PARTICLE_TO].force.horizontal = EE_PARTICLE_FORCE_H;
    forces[EE_PARTICLE_FROM].force.vertical = forces[EE_PARTICLE_TO].force.vertical = EE_PARTICLE_FORCE_V;
    forces[EE_PARTICLE_FROM].gravity.horizontal = forces[EE_PARTICLE_TO].gravity.horizontal = EE_PARTICLE_FORCE_H;
    forces[EE_PARTICLE_FROM].gravity.vertical = forces[EE_PARTICLE_TO].gravity.vertical = EE_PARTICLE_FORCE_V;
    forces[EE_PARTICLE_FROM].angular = forces[EE_PARTICLE_TO].angular = EE_PARTICLE_ROTATION;
    color[EE_PARTICLE_FROM].red = color[EE_PARTICLE_FROM].green = color[EE_PARTICLE_FROM].blue = color[EE_PARTICLE_FROM].alpha = 255;
    color[EE_PARTICLE_TO].red = color[EE_PARTICLE_TO].green = color[EE_PARTICLE_TO].blue = color[EE_PARTICLE_TO].alpha = 255;
    conclusive.red = conclusive.green = conclusive.blue = 255;
    conclusive.alpha = 0;
    /* activate subpixel movement */
    this->subpixel = true;
}

int cparticle::initialize (ctarga *master, size_t elements) {
    size_t spawning;
    int result = EE_OK;
    sparticle *particle;
    this->master = master;
    this->elements = elements;
    coordinates[EPOSITIONSUBJECT_ELEMENT].destination[EPOSITION_WIDTH] = master->destination(EPOSITION_WIDTH, EPOSITIONSUBJECT_ELEMENT);
    coordinates[EPOSITIONSUBJECT_ELEMENT].destination[EPOSITION_HEIGHT] = master->destination(EPOSITION_HEIGHT, EPOSITIONSUBJECT_ELEMENT);
    coordinates[EPOSITIONSUBJECT_ELEMENT].source[EPOSITION_POSITIONX] = master->source(EPOSITION_POSITIONX, EPOSITIONSUBJECT_ELEMENT);
    coordinates[EPOSITIONSUBJECT_ELEMENT].source[EPOSITION_POSITIONY] = master->source(EPOSITION_POSITIONY, EPOSITIONSUBJECT_ELEMENT);
    coordinates[EPOSITIONSUBJECT_ELEMENT].source[EPOSITION_WIDTH] = master->source(EPOSITION_WIDTH, EPOSITIONSUBJECT_ELEMENT);
    coordinates[EPOSITIONSUBJECT_ELEMENT].source[EPOSITION_HEIGHT] = master->source(EPOSITION_HEIGHT, EPOSITIONSUBJECT_ELEMENT);
    coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_WIDTH] = master->source(EPOSITION_WIDTH, EPOSITIONSUBJECT_ELEMENT);
    coordinates[EPOSITIONSUBJECT_FRAME].source[EPOSITION_HEIGHT] = master->source(EPOSITION_HEIGHT, EPOSITIONSUBJECT_ELEMENT);
    if (particles.records() < elements) {
        for (spawning = elements - particles.records(); spawning > 0; spawning--) {
            if ((particle = (sparticle *) emalloc(sizeof(sparticle)))) {
                generate(particle);
                if (!(result = particles.add(particle)))
                    break;
            } else
                ekill("out of memory");
        }
    } else if (particles.records() > elements) {
        for (spawning = particles.records() - elements; spawning > 0; spawning--) {
            if ((particle = particles.get((size_t)0))) {
                particle->stopwatch.unload();
                efree(particle);
                particles.del();
            }
        }
    }
    return result;
}

int cparticle::initialize (cparticle *context) {
    rotation = context->rotation;
    flipped[EFLIPDIRECTION_HORIZONTAL] = context->flipped[EFLIPDIRECTION_HORIZONTAL];
    flipped[EFLIPDIRECTION_VERTICAL] = context->flipped[EFLIPDIRECTION_VERTICAL];
    position[EE_PARTICLE_FROM].overflow[EPOSITION_POSITIONX] = context->position[EE_PARTICLE_FROM].overflow[EPOSITION_POSITIONX];
    position[EE_PARTICLE_FROM].overflow[EPOSITION_POSITIONY] = context->position[EE_PARTICLE_FROM].overflow[EPOSITION_POSITIONY];
    position[EE_PARTICLE_TO].overflow[EPOSITION_POSITIONX] = context->position[EE_PARTICLE_TO].overflow[EPOSITION_POSITIONX];
    position[EE_PARTICLE_TO].overflow[EPOSITION_POSITIONY] = context->position[EE_PARTICLE_TO].overflow[EPOSITION_POSITIONY];
    position[EE_PARTICLE_FROM].zoom[EPOSITION_WIDTH] = context->position[EE_PARTICLE_FROM].zoom[EPOSITION_WIDTH];
    position[EE_PARTICLE_FROM].zoom[EPOSITION_HEIGHT] = context->position[EE_PARTICLE_FROM].zoom[EPOSITION_HEIGHT];
    position[EE_PARTICLE_TO].zoom[EPOSITION_WIDTH] = context->position[EE_PARTICLE_TO].zoom[EPOSITION_WIDTH];
    position[EE_PARTICLE_TO].zoom[EPOSITION_HEIGHT] = context->position[EE_PARTICLE_TO].zoom[EPOSITION_HEIGHT];
    life[EE_PARTICLE_FROM].regenerations = context->life[EE_PARTICLE_FROM].regenerations;
    life[EE_PARTICLE_TO].regenerations = context->life[EE_PARTICLE_TO].regenerations;
    life[EE_PARTICLE_FROM].ttl = context->life[EE_PARTICLE_FROM].ttl;
    life[EE_PARTICLE_FROM].ttu[EPOSITION_POSITIONX] = context->life[EE_PARTICLE_FROM].ttu[EPOSITION_POSITIONX];
    life[EE_PARTICLE_FROM].ttu[EPOSITION_POSITIONY] = context->life[EE_PARTICLE_FROM].ttu[EPOSITION_POSITIONY];
    life[EE_PARTICLE_FROM].maxttu = context->life[EE_PARTICLE_FROM].maxttu;
    life[EE_PARTICLE_TO].ttl = context->life[EE_PARTICLE_TO].ttl;
    life[EE_PARTICLE_TO].ttu[EPOSITION_POSITIONX] = context->life[EE_PARTICLE_TO].ttu[EPOSITION_POSITIONX];
    life[EE_PARTICLE_TO].ttu[EPOSITION_POSITIONY] = context->life[EE_PARTICLE_TO].ttu[EPOSITION_POSITIONY];
    life[EE_PARTICLE_TO].maxttu = context->life[EE_PARTICLE_TO].maxttu;
    forces[EE_PARTICLE_FROM].force.horizontal = context->forces[EE_PARTICLE_FROM].force.horizontal;
    forces[EE_PARTICLE_FROM].force.vertical = context->forces[EE_PARTICLE_FROM].force.vertical;
    forces[EE_PARTICLE_TO].force.horizontal = context->forces[EE_PARTICLE_TO].force.horizontal;
    forces[EE_PARTICLE_TO].force.vertical = context->forces[EE_PARTICLE_TO].force.vertical;
    forces[EE_PARTICLE_FROM].gravity.horizontal = context->forces[EE_PARTICLE_FROM].gravity.horizontal;
    forces[EE_PARTICLE_FROM].gravity.vertical = context->forces[EE_PARTICLE_FROM].gravity.vertical;
    forces[EE_PARTICLE_TO].gravity.horizontal = context->forces[EE_PARTICLE_TO].gravity.horizontal;
    forces[EE_PARTICLE_TO].gravity.vertical = context->forces[EE_PARTICLE_TO].gravity.vertical;
    forces[EE_PARTICLE_FROM].angular = context->forces[EE_PARTICLE_FROM].angular;
    forces[EE_PARTICLE_TO].angular = context->forces[EE_PARTICLE_TO].angular;
    color[EE_PARTICLE_TO] = context->color[EE_PARTICLE_TO];
    color[EE_PARTICLE_FROM] = context->color[EE_PARTICLE_FROM];
    conclusive = context->conclusive;
    elements = context->elements;
    return initialize(context->master, elements);
}

void cparticle::generate (sparticle *particle, bool already) {
    unsigned int red, green, blue, alpha;
    if (!already) {
        particle->life.regenerations = 0;
        particle->expired = false;
        particle->stopwatch.init();
        particle->stopwatch.add("d");
        particle->stopwatch.add("c");
        particle->stopwatch.add("mx");
        particle->stopwatch.add("my");
    } else {
        if (life[EE_PARTICLE_FROM].regenerations > 0) {
            particle->life.regenerations++;
            if (particle->life.regenerations > life[EE_PARTICLE_FROM].regenerations)
                particle->expired = true;
        }
        particle->stopwatch.set("d");
        particle->stopwatch.set("c");
        particle->stopwatch.add("mx");
        particle->stopwatch.add("my");
    }
    if (!particle->expired) {
        particle->forces.force.vertical = 0;
        particle->forces.force.horizontal = 0;
        erandrange(color[EE_PARTICLE_TO].red, color[EE_PARTICLE_FROM].red, particle->color.red);
        erandrange(color[EE_PARTICLE_TO].green, color[EE_PARTICLE_FROM].green, particle->color.green);
        erandrange(color[EE_PARTICLE_TO].blue, color[EE_PARTICLE_FROM].blue, particle->color.blue);
        erandrange(color[EE_PARTICLE_TO].alpha, color[EE_PARTICLE_FROM].alpha, particle->color.alpha);
        erandrange(life[EE_PARTICLE_TO].ttl, life[EE_PARTICLE_FROM].ttl, particle->life.ttl);
        erandrange(life[EE_PARTICLE_TO].ttu[EPOSITION_POSITIONX], life[EE_PARTICLE_FROM].ttu[EPOSITION_POSITIONX], particle->life.ttu[EPOSITION_POSITIONX]);
        erandrange(life[EE_PARTICLE_TO].ttu[EPOSITION_POSITIONY], life[EE_PARTICLE_FROM].ttu[EPOSITION_POSITIONY], particle->life.ttu[EPOSITION_POSITIONY]);
        erandrange(life[EE_PARTICLE_TO].maxttu, life[EE_PARTICLE_FROM].maxttu, particle->life.maxttu);
        red = ((float)particle->life.ttl)/EE_MAX(abs(conclusive.red-particle->color.red), 1);
        green = ((float)particle->life.ttl)/EE_MAX(abs(conclusive.green-particle->color.green), 1);
        blue = ((float)particle->life.ttl)/EE_MAX(abs(conclusive.blue-particle->color.blue), 1);
        alpha = ((float)particle->life.ttl)/EE_MAX(abs(conclusive.alpha-particle->color.alpha), 1);
        particle->jump = EE_MIN(EE_MIN(red, green), EE_MIN(blue, alpha));
        erandrange(forces[EE_PARTICLE_TO].gravity.vertical, forces[EE_PARTICLE_FROM].gravity.vertical, particle->forces.gravity.vertical);
        erandrange(forces[EE_PARTICLE_TO].gravity.horizontal, forces[EE_PARTICLE_FROM].gravity.horizontal, particle->forces.gravity.horizontal);
        erandrange(abs(forces[EE_PARTICLE_TO].force.vertical), abs(forces[EE_PARTICLE_FROM].force.vertical), particle->maximum.vertical);
        erandrange(abs(forces[EE_PARTICLE_TO].force.horizontal), abs(forces[EE_PARTICLE_FROM].force.horizontal), particle->maximum.horizontal);
        particle->factor[EPOSITION_POSITIONX] = 1.0/(particle->maximum.horizontal);
        particle->factor[EPOSITION_POSITIONY] = 1.0/(particle->maximum.vertical);
        erandrange(destination(EPOSITION_POSITIONX)+position[EE_PARTICLE_TO].overflow[EPOSITION_POSITIONX], destination(EPOSITION_POSITIONX)+position[EE_PARTICLE_FROM].overflow[EPOSITION_POSITIONX], particle->position.overflow[EPOSITION_POSITIONX]);
        erandrange(destination(EPOSITION_POSITIONY)+position[EE_PARTICLE_TO].overflow[EPOSITION_POSITIONY], destination(EPOSITION_POSITIONY)+position[EE_PARTICLE_FROM].overflow[EPOSITION_POSITIONY], particle->position.overflow[EPOSITION_POSITIONY]);
        efrandrange(position[EE_PARTICLE_TO].zoom[EPOSITION_WIDTH], position[EE_PARTICLE_FROM].zoom[EPOSITION_WIDTH], particle->position.zoom[EPOSITION_WIDTH]);
        efrandrange(position[EE_PARTICLE_TO].zoom[EPOSITION_HEIGHT], position[EE_PARTICLE_FROM].zoom[EPOSITION_HEIGHT], particle->position.zoom[EPOSITION_HEIGHT]);
        efrandrange(forces[EE_PARTICLE_TO].angular, forces[EE_PARTICLE_FROM].angular, particle->forces.angular);
    }
}

void cparticle::refresh (sparticle *particle) {
    float color[ECOLOR_NULL];
    int force, value;
    bool update[EPOSITION_NULL] = {false, false, false, false};
    if (!particle->expired) {
        if (particle->stopwatch.get("mx").usecs > particle->life.ttu[EPOSITION_POSITIONX]) {
            particle->stopwatch.set("mx");
            update[EPOSITION_POSITIONX] = true;
        }
        if (particle->stopwatch.get("my").usecs > particle->life.ttu[EPOSITION_POSITIONY]) {
            particle->stopwatch.set("my");
            update[EPOSITION_POSITIONY] = true;
        }
        if (particle->stopwatch.get("c").usecs > particle->jump) {
            particle->stopwatch.set("c");
            estep(conclusive.red, particle->color.red, 1);
            estep(conclusive.green, particle->color.green, 1);
            estep(conclusive.blue, particle->color.blue, 1);
            estep(conclusive.alpha, particle->color.alpha, 1);
        }
        coordinates[EPOSITIONSUBJECT_BEFORE].destination[EPOSITION_POSITIONX] = particle->position.overflow[EPOSITION_POSITIONX];
        coordinates[EPOSITIONSUBJECT_BEFORE].destination[EPOSITION_POSITIONY] = particle->position.overflow[EPOSITION_POSITIONY];
        if (update[EPOSITION_POSITIONX]) {
            erandrange(forces[EE_PARTICLE_TO].force.horizontal, forces[EE_PARTICLE_FROM].force.horizontal, force);
            value = (particle->forces.force.horizontal*particle->forces.gravity.horizontal);
            if (value < 0) {
                particle->life.ttu[EPOSITION_POSITIONX] += abs(particle->forces.gravity.horizontal*EE_PARTICLE_TSTEP);
                if (particle->life.ttu[EPOSITION_POSITIONX] > particle->life.maxttu)
                    particle->forces.force.horizontal += ((particle->forces.gravity.horizontal)>=0)?1:-1;
            } else if (value == 0) {
                particle->life.ttu[EPOSITION_POSITIONX] += abs(particle->forces.gravity.horizontal*EE_PARTICLE_TSTEP);
                if (particle->life.ttu[EPOSITION_POSITIONX] >= particle->life.maxttu)
                    particle->life.ttu[EPOSITION_POSITIONX] = particle->life.maxttu;
            } else {
                if (particle->forces.force.horizontal < abs(particle->maximum.horizontal))
                    particle->forces.force.horizontal += ((particle->forces.gravity.horizontal)>=0)?1:-1;
                value = abs(particle->forces.gravity.horizontal*EE_PARTICLE_TSTEP);
                if (particle->life.ttu[EPOSITION_POSITIONX] > value)
                    particle->life.ttu[EPOSITION_POSITIONX] -= value;
            }
            particle->forces.force.horizontal = (force*(((particle->forces.force.horizontal*force)<0)?-1:1));
            particle->position.overflow[EPOSITION_POSITIONX] += particle->forces.force.horizontal;
        }
        if (update[EPOSITION_POSITIONY]) {
            erandrange(forces[EE_PARTICLE_TO].force.vertical, forces[EE_PARTICLE_FROM].force.vertical, force);
            value = (particle->forces.force.vertical*particle->forces.gravity.vertical);
            if (value < 0) {
                particle->life.ttu[EPOSITION_POSITIONY] += abs(particle->forces.gravity.vertical*EE_PARTICLE_TSTEP);
                if (particle->life.ttu[EPOSITION_POSITIONY] > particle->life.maxttu)
                    particle->forces.force.vertical *= -1;
            } else if (value == 0) {
                particle->life.ttu[EPOSITION_POSITIONY] += abs(particle->forces.gravity.vertical*EE_PARTICLE_TSTEP);
                if (particle->life.ttu[EPOSITION_POSITIONY] >= particle->life.maxttu)
                    particle->life.ttu[EPOSITION_POSITIONY] = particle->life.maxttu;
            } else {
                value = abs(particle->forces.gravity.vertical*EE_PARTICLE_TSTEP);
                if (particle->life.ttu[EPOSITION_POSITIONY] > value)
                    particle->life.ttu[EPOSITION_POSITIONY] -= value;
            }
            particle->forces.force.vertical = (force*(((particle->forces.force.vertical*force)<0)?-1:1));
            particle->position.overflow[EPOSITION_POSITIONY] += particle->forces.force.vertical;
        }
        particle->rotation += particle->forces.angular;
        coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_POSITIONX] = particle->position.overflow[EPOSITION_POSITIONX];
        coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_POSITIONY] = particle->position.overflow[EPOSITION_POSITIONY];
        coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_WIDTH] = ((float)master->destination(EPOSITION_WIDTH, EPOSITIONSUBJECT_FRAME)*(float)particle->position.zoom[EPOSITION_WIDTH]);
        coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_HEIGHT] = ((float)master->destination(EPOSITION_HEIGHT, EPOSITIONSUBJECT_FRAME)*(float)particle->position.zoom[EPOSITION_HEIGHT]);
        coordinates[EPOSITIONSUBJECT_BEFORE].destination[EPOSITION_WIDTH] = coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_WIDTH];
        coordinates[EPOSITIONSUBJECT_BEFORE].destination[EPOSITION_HEIGHT] = coordinates[EPOSITIONSUBJECT_FRAME].destination[EPOSITION_HEIGHT];
        color[ECOLOR_R] = (((float)particle->color.red)/255.0f);
        color[ECOLOR_G] = (((float)particle->color.green)/255.0f);
        color[ECOLOR_B] = (((float)particle->color.blue)/255.0f);
        color[ECOLOR_A] = (((float)particle->color.alpha)/255.0f);
        channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_R] = mastercolor(color[ECOLOR_R], channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_R]);
        channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_G] = mastercolor(color[ECOLOR_G], channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_G]);
        channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_B] = mastercolor(color[ECOLOR_B], channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_B]);
        channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_A] = mastercolor(color[ECOLOR_A], channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_A]);
        rotation = particle->rotation;
    }
}

scolor *cparticle::pixel (unsigned int positionx, unsigned int positiony) {
    scolor *result = NULL;
    if (master)
        result = master->pixel(positionx, positiony);
    return result;
}

GLuint cparticle::get (void) {
    GLuint texture = EE_NULLTEXTURE;
    sparticle *particle = NULL;
    if (master) {
        while ((particle = particles.nextvalue()))
            if (!particle->expired) {
                if (particle->stopwatch.get("d").usecs < particle->life.ttl) {
                    refresh(particle);
                    singletexture(texture, master);
                    break;
                } else
                    generate(particle, true);
            }
    }
    return texture;
}

void cparticle::restore (void) {
    if (master)
        master->restore();
    channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_R] = channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_G] = channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_B] = channels[EPOSITIONSUBJECT_FRAME].color[ECOLOR_A] = 1.0f;
    channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_R] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_G] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_B] = channels[EPOSITIONSUBJECT_ELEMENT].color[ECOLOR_A] = 1.0f;
    rotation = 0.0f;
}

void cparticle::reset (void) {
    sparticle *particle = NULL;
    foreach(particle, &particles) {
        particle->life.regenerations = 0;
        particle->expired = false;
    }
}

void cparticle::unload (sparticle *particle) {
    particle->stopwatch.unload();
    efree(particle);
}

void cparticle::unload (void) {
    destroy(&particles, unload, sparticle);
    this->init();
}