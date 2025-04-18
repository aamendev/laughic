#ifndef SCENE_H
#define SCENE_H
#include "../math/traceable.h"
#include "../types.h"
#include "../graphics_util.h"
#include "../logic_util.h"
#include "../shapes.h"
#include "../samplers/sampler.h"
#include "../cameras/camera.h"
#include "../math/aabb.h"
typedef struct SceneData
{
    Canvas* canvas;
    Sampler* sampler;
    u32 default_colour;
    Ray* ray;
    float pixelSize;
    Traceable** traceables;    
    int traceable_count;
    Camera* cam;
    int optimized;
} SceneData;

typedef struct Scene
{
    SceneData* scene_data;
    void (*ray_trace)(struct SceneData* scene);
} Scene;

void simple_tracer(SceneData* scene);
void perspective_tracer(SceneData* scene);
void optimized_perspective_tracer(SceneData* scene);
void unoptimized_perspective_tracer(SceneData* scene);
void construct_bvh(SceneData* sc, struct BVH*);
void combine_bvh(struct BVH*, struct BVH*);
#endif
