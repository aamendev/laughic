#ifndef AABB_H
#define AABB_H
#include "vector3d.h"
#include "ray.h"
#include "traceable.h"

typedef struct AABB
{
    Vector3d min_coord;
    Vector3d max_coord;
} AABB;
int aabb_intersects(Ray* ray, void* data);

#define AABB_TRACE(b, c) ((Traceable)\
        {\
            .data = &b,\
            .col = c,\
            .intersects = aabb_intersects\
        }) 

#endif
