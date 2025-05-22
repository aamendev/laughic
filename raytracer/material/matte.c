#include "matte.h"

u32 matte_shade(void* data)
{
    MatteData* d = (MatteData*)data;
    MaterialData* md = (MaterialData*)d->material_data;

    Vector3d w0 = scale(&d->ray->direction, -1);
    u32 temp = d->ambient_brdf->rho(d->ambient_brdf->data, &w0); 
    u8 temp_comp[4];
    unpack(temp_comp, &temp);
    u32 ambient = md->ambient_light->get_radiance(md->ambient_light->data);
    u8 ambient_comp[4];
    unpack(ambient_comp, &ambient);
    for (int i = 0; i < 3; i++)
    {
        float fac = (f32)ambient_comp[i] / 0xff;
        temp_comp[i] *= fac;
    }
    return 0;
}
