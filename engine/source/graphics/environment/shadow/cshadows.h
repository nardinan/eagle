/*
 * PSYCHO GAMES(c) STUDIOS-2008-2012 http://www.psychogames.net/
 *
 * PROJECT      : eagle engine
 * DEVELOPER    : Andrea Nardinocchi
 * EMAIL        : nardinocchi@psychogames.net
 *
 * Copyright (c) 2011 psycho games studios. All rights reserved.
 */
#ifndef EE_CSHADOWS_H
#define EE_CSHADOWS_H
#include "ccollision.h"
#include "cenvironment.h"
#define EE_SEGMENT_ONE 0
#define EE_SEGMENT_TWO 1
#define EE_SEGMENTS 2
typedef struct ssilhouette {
    spoint source, destination, projection[2];
    float alpha[2];
} ssilhouette;
typedef struct sshadow {
    ccontext *context;
    carray<spoint> convexhull;
    carray<ssilhouette> shape;
} sshadow;
class cshadows:public eagleclass {
friend class clights;
private:
    carray<sshadow> shadows;
public:
    inline cshadows (void) { this->init(); };
    void init (void);
    int add (ccontext *context, const char *label);
    int vertex (const char *label, spoint coordinate);
    inline sshadow *get (const char *label) { return shadows.get(label); };
    void del (const char *label);
private:
    static int dot (spoint left, spoint right);
    static spoint project (spoint point, spoint light);
    static void recalculate (sshadow *shadow, spoint light, unsigned int intensity, float base, spoint source, spoint destination);
    static void recalculate (sshadow *shadow, spoint light, unsigned int intensity, float base);
    void unload (sshadow *shadow);
public:
    void unload (void);
};
#endif
