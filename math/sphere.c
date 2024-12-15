#include "sphere.h"
#include "vector3d.h"

/*int intersects(Ray *r, Sphere *s)
{
    Vector3d base_center = sub(&(s->center), &(r->base));
    if (dot(&base_center, &(r->direction)) < 0 &&
            squared_magnitude(&base_center) > s->r * s->r)
        return 0;
    Vector3d rejected = reject(&base_center, &(r->direction));
    return magnitude(&rejected) <= s->r;
}
*/

int sphere_intersects(Ray *r, void *data)
{
    float rad = ((Sphere*)data)->r;
    Vector3d center = ((Sphere*)data)->center;

    Vector3d base_center = sub(&(center), &(r->base));
    if (dot(&base_center, &(r->direction)) < 0 &&
            squared_magnitude(&base_center) > rad * rad)
        return 0;
    Vector3d rejected = reject(&base_center, &(r->direction));
    return magnitude(&rejected) <= rad;
}
