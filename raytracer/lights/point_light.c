#include "point_light.h"
#include "light.h"

Ray* pl_get_rays(Vector3d* in_position, void* data, u32* count)
{
    PointLight* pl = (PointLight*)data;
    Ray* ray = malloc(sizeof(Ray));
    ray->base = pl->pos;
    ray->direction = sub(in_position, &pl->pos);
    ray->direction = normalise(&ray->direction);
    *count = 1;
    return ray;
}

u32 pl_get_radiance(void* data)
{
    PointLight* pl = (PointLight*)data;
    u8 comps[4];
    unpack(comps, &pl->colour);
    for (int i = 0; i < 3; i++)
    {
        comps[i] *= pl->ls;
    }
    u32 ret_colour = 0;
    pack(comps, &ret_colour); 
    return ret_colour;
}
