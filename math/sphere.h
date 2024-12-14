#ifndef SPHERE_H
#define SPHERE_H
#include "vector3d.h"
#include "ray.h"
#include "traceable.h"

typedef struct SphereData
{
    float r;
    Vector3d center;
} SphereData;

typedef struct Sphere
{
    Traceable* traceable;
    
    SphereData data;
} Sphere;

int sphere_intersects(Ray* r, void* data);

#endif
