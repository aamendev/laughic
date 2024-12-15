#ifndef SPHERE_H
#define SPHERE_H
#include "vector3d.h"
#include "ray.h"
#include "traceable.h"

typedef struct Sphere
{
    float r;
    Vector3d center;
} Sphere;

int sphere_intersects(Ray* r, void* data);

#define SPHERE_TRACE(s, c) ((Traceable)\
        {\
        .data = &s,\
        .col = c,\
        .intersects = sphere_intersects\
        })

#endif
