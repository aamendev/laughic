#ifndef MATERIAL_H
#define MATERIAL_H
#include "../brdf/brdf.h"
#include "../../managers/scene.h"
typedef struct Material
{
    void* data;
    char* name;
    u32 (*shade)(void* data);
} Material;
#endif
