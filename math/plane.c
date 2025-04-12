#include "./plane.h"
#include "vector3d.h"

int plane_intersects(Ray *ray, void *data)
{
    Plane* p = (Plane*)data;
    float fv = dot(&p->normal, &ray->direction);
    return fabs(fv) > FLT_MIN;
}
