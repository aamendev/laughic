#include "aabb.h"
#include "vector3d.h"
#include <math.h>
#include <stdio.h>

void fswap (float* f1, float* f2)
{
    float temp;
    temp = *f1;
    *f1 = *f2;
    *f2 = temp;
}

static f32 min(f32 a, f32 b)
{
    return a * (a < b) + b * !(a < b);
}

static f32 max(f32 a, f32 b)
{
    return a * (a > b) + b * !(a > b);
}

float aabb_intersects(Ray* ray, void* data)
{
    float t_near, t_far, t0, t1;

    AABB* box = (AABB*)data;  

    t0 = (box->min_coord.x - ray->base.x) * (1 / ray->direction.x);
    t1 = (box->max_coord.x - ray->base.x) * (1 / ray->direction.x);

    t_near = min(t0, t1);
    t_far = max(t0, t1);

    t0 = (box->min_coord.y - ray->base.y) * (1 / ray->direction.y);
    t1 = (box->max_coord.y - ray->base.y) * (1 / ray->direction.y);

    t_near = max(t_near, min(t0, t1));
    t_far = min(t_far, max(t0, t1));

    t0 = (box->min_coord.z - ray->base.z) * (1 / ray->direction.z);
    t1 = (box->max_coord.z - ray->base.z) * (1 / ray->direction.z);

    t_near = max(t_near, min(t0, t1));
    t_far = min(t_far, max(t0, t1));
    int cond = (t_far >= max(0.0f, t_near));
    return t_near * (cond) - !cond;
}
void aabb_get_bounding_extents(Traceable* t, Vector3d* min, Vector3d* max)
{
    AABB* box = (AABB*)t->data;
    *min = box->min_coord;
    *max = box->max_coord;
}
void aabb_get_bvh(Traceable* t, struct BVH* bvh)
{
    bvh->root = t;
    bvh->left = NULL;
    bvh->right = NULL;
}
