#ifndef SCENE_H
#define SCENE_H
#include "../math/traceable.h"
#include "../types.h"
#include "../graphics_util.h"
#include "../logic_util.h"
#include "../shapes.h"
typedef struct SceneData
{
    Canvas* canvas;
    u32 default_colour;
    Ray* ray;
    int samples;
    float pixelSize;
    Traceable** traceables;    
    int traceable_count;
} SceneData;

typedef struct Scene
{
    SceneData* scene_data;
    void (*ray_trace)(struct SceneData* scene);
} Scene;

void simple_tracer(SceneData* scene);
#endif
