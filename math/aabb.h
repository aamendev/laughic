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
float aabb_intersects(Ray* ray, void* data);
void aabb_get_bounding_extents(Traceable* t, Vector3d* min, Vector3d* max);
void aabb_get_bvh(Traceable* t, struct BVH* bvh);

#define AABB_TRACE(b, c, n) ((Traceable)\
        {\
            .data = b,\
            .col = c,\
            .name=  n,\
            .intersects = aabb_intersects,\
            .get_bounding_extents = aabb_get_bounding_extents,\
            .get_bvh = aabb_get_bvh\
        }) 

#endif
