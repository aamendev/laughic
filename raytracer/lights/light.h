#ifndef LIGHT_H
#define LIGHT_H
#include "../../math/vector3d.h"
#include "../../graphics_util.h"
#include "../../logic_util.h"
#include "../../types.h"
#include "../../math/ray.h"
typedef struct Light
{
   void* data;
   char* name;
   Ray* (*get_rays)(Vector3d* in_position, void* data, u32* count);
   u32 (*get_radiance)(void* data);
}Light;
#endif
