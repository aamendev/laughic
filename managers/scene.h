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
#include "../raytracer/lights/light.h"
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
    Light* lights;
    u32 light_count;
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
void ligh_tracer(SceneData* data);
void construct_bvh(SceneData* sc, struct BVH*);
void combine_bvh(struct BVH*, struct BVH*);
void check_bvh_intersection(struct BVH*, Ray* ray, u32 trace_count, 
        i32* currMin, 
        f32* currMinVal, Vector3d* curr_normal);
#endif
