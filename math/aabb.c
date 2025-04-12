#include "aabb.h"
#include "vector3d.h"
#include <math.h>

void fswap (float* f1, float* f2)
{
    float temp;
    temp = *f1;
    *f1 = *f2;
    *f2 = temp;
}

int aabb_intersects(Ray* ray, void* data)
{
    float t_near, t_far;
    t_near = -INFINITY;
    t_far = INFINITY;

    AABB* box = (AABB*)data;  
    Vector3d* curr_min = ray->direction.x > 0 ? &box->min_coord : &box->max_coord;
    Vector3d* curr_max = ray->direction.x > 0 ? &box->max_coord : &box->min_coord;
    float t0 = (curr_min->x - ray->base.x) * (1 / ray->direction.x);
    float t1 = (curr_max->x - ray->base.x) * (1 / ray->direction.x);
    
    t_near = t0;
    t_far = t1;
    if (t_far < t_near || t_far < 0) return 0;

    curr_min = ray->direction.y > 0 ? &box->min_coord : &box->max_coord;
    curr_max = ray->direction.y > 0 ? &box->max_coord : &box->min_coord;
    t0 = (curr_min->y - ray->base.y) * (1 / ray->direction.y);
    t1 = (curr_max->y - ray->base.y) * (1 / ray->direction.y);

    t_near = t_near * (t_near > t0) + t0 * !(t_near > t0);
    t_far = t_far * (t_far < t1) + t1 * !(t_far < t1);
    if (t_far < t_near || t_far < 0) return 0;

    curr_min = ray->direction.z > 0 ? &box->min_coord : &box->max_coord;
    curr_max = ray->direction.z > 0 ? &box->max_coord : &box->min_coord;
    t0 = (curr_min->z - ray->base.z) * (1 / ray->direction.z);
    t1 = (curr_max->z - ray->base.z) * (1 / ray->direction.z);

    t_near = t_near * (t_near > t0) + t0 * !(t_near > t0);
    t_far = t_far * (t_far < t1) + t1 * !(t_far < t1);
    return (t_near <= t_far && t_far > 0);
}
