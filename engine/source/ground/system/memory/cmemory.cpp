/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#include "cmemory.h"
unsigned long int memory = 0;
sallocation *allocations = NULL;
void *eemalloc (size_t size, const char *file, unsigned int line) {
    scoordinates *coordinate = NULL;
    sallocation *singleton = NULL;
    void *result = NULL;
    unsigned char *checksum = NULL;
    size_t length = (size+sizeof(scoordinates))+EE_OVERFLOW;
    if ((result = malloc(length))) {
        checksum = (unsigned char *)((char *)result+size);
        coordinate = (scoordinates *)(checksum+EE_OVERFLOW);
        coordinate->file = file;
        coordinate->line = line;
        memset(result, 0, size);
        *((unsigned int *)checksum) = EE_CHECKSUM;
        if ((singleton = (sallocation *) malloc(sizeof(sallocation)))) {
            if (allocations)
                allocations->back = singleton;
            singleton->pointer = result;
            singleton->size = size;
            singleton->next = allocations;
            singleton->back = NULL;
            allocations = singleton;
            memory += length;
        } else
            fprintf(stderr, "memory in overallocation at %s (line %d)\n", file, line);
    } else
        fprintf(stderr, "memory in overallocation at %s (line %d)\n", file, line);
    return result;
}

void *eerealloc (void *pointer, size_t size, const char *file, unsigned int line) {
    sallocation *singleton = allocations;
    void *result = NULL;
    if ((result = eemalloc(size, file, line))) {
        while (singleton) {
            if (singleton->pointer == pointer) {
                memcpy(result, pointer, (singleton->size>size)?size:singleton->size);
                efree(pointer);
                break;
            } else
                singleton = singleton->next;
        }
    } else
        fprintf(stderr, "memory in overallocation\n");
    return result;
}

void eefree (void *pointer, const char *file, unsigned int line) {
    scoordinates *coordinate = NULL;
    sallocation *singleton = allocations;
    unsigned char *checksum = NULL;
    bool done = false;
    size_t length;
    while (singleton) {
        if (singleton->pointer == pointer) {
            length = (singleton->size+sizeof(scoordinates))+EE_OVERFLOW;
            checksum = (unsigned char *)(((char *)singleton->pointer)+singleton->size);
            if (*((unsigned int *)checksum) != EE_CHECKSUM) {
                coordinate = (scoordinates *)(checksum+EE_OVERFLOW);
                fprintf(stderr, "mistaken pointer allocated at %s (line %d) and free'd at %s (line %d)\n", coordinate->file, coordinate->line, file, line);
            }
            if (singleton->next)
                singleton->next->back = singleton->back;
            if (singleton->back)
                singleton->back->next = singleton->next;
            else
                allocations = singleton->next;
            free(singleton);
            free(pointer);
            memory -= length;
            done = true;
            break;
        }
        singleton = singleton->next;
    }
    if (!done)
        fprintf(stderr, "suspicious of double free called at %s (line %d)\n", file, line);
}
