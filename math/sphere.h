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

float sphere_intersects(Ray* r, void* data);
void sphere_get_bounding_extents(Traceable* t, Vector3d* min, Vector3d* max);
void sphere_get_bvh(Traceable* t, struct BVH* bvh);

#define SPHERE_TRACE(s, c, n) ((Traceable)\
        {\
        .data = s,\
        .col = c,\
        .name = n,\
        .intersects = sphere_intersects,\
        .get_bounding_extents = sphere_get_bounding_extents,\
        .get_bvh = sphere_get_bvh\
        })

#endif
