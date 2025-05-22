#ifndef MATTE_H
#define MATTE_H
#include "material.h"

typedef struct MatteData
{
    BRDF* ambient_brdf;
    BRDF* diffuse_brdf;
    Ray* ray;
    MaterialData* material_data;
} MatteData;

u32 matte_shade(void* data);
#define MATTE_MATERIAL(s, n) ((Material)\
        {\
        .data = s,\
        .name = n,\
        .shade = matte_shader\
        })
#endif
