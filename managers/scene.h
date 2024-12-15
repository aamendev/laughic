#ifndef SCENE_H
#define SCENE_H
#include "../math/traceable.h"
#include "../types.h"
#include "../graphics_util.h"
#include "../logic_util.h"
#include "../shapes.h"
#include "../samplers/sampler.h"
typedef struct SceneData
{
    Canvas* canvas;
    Sampler* sampler;
    u32 default_colour;
    Ray* ray;
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
void perspective_tracer(SceneData* scene);
#endif
