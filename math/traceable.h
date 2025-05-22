#ifndef TRACEABLE_H
#define TRACEABLE_H
#include "ray.h"
#include "../types.h"
#include "vector3d.h"
#include "../colours.h"
#include "../raytracer/brdf/brdf.h"

typedef struct Traceable
{
    struct BVH
    {
        struct Traceable* root;
        struct BVH* left;
        struct BVH* right;
    } m_bvh;

    void* data;
    char* name;
    u32 col;
    int id;
    BRDF ambient;
    BRDF diffuse;
    BRDF specular;
    float (*intersects)(Ray* r, void* data, Vector3d* normal);
    void(*get_bounding_extents)(struct Traceable* t, Vector3d* min, Vector3d* max);
    void(*get_bvh)(struct Traceable* t, struct BVH* bvh);
} Traceable;


#endif
