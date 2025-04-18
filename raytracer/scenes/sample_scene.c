#include "sample_scene.h"

void rect_scene(Canvas* canvas)
{
    Vector3d rayDirection = {.x = 0, .y = 0, .z = -1};
    Vector3d initPosition = {.x = 0, .y = 0, .z = 100};
    Ray r = {.direction = rayDirection, .base = initPosition};

    Sphere s = {.center = {.x = 0, .y = -30, .z = 0}, .r = 50.0f};
    AABB floor = 
    {
        .min_coord = {.x = -200, .y = 100, .z = -200},
        .max_coord = {.x = 200, .y = 100, .z = 200},
    };
    AABB front = 
    {
        .min_coord = {.x = 0, .y = 0, .z = 50}, 
        .max_coord = {.x = 100, .y = 100, .z = 50}
    };
    AABB right = 
    {
        .min_coord = {.x = 100, .y = 0, .z = 0}, 
        .max_coord = {.x = 100, .y = 100, .z = 50}
    };
    AABB back = 
    {
        .min_coord = {.x = 0, .y = 0, .z = 0}, 
        .max_coord = {.x = 100, .y = 100, .z = 0}
    };

    AABB left = 
    {
        .min_coord = {.x = 0, .y = 0, .z = 0}, 
        .max_coord = {.x = 0, .y = 100, .z = 50}
    };

    Traceable frontTrace = AABB_TRACE(&front, YELLOW, "bf");
    Traceable rightTrace = AABB_TRACE(&right, RED, "br");
    Traceable backTrace = AABB_TRACE(&back, GREEN, "bb");
    Traceable leftTrace = AABB_TRACE(&left, BLUE, "bl");
    Traceable floorTrace = AABB_TRACE(&floor, WHITE, "floor");
    Traceable ball = SPHERE_TRACE(&s, CYAN, "ball");
    Traceable* traceables[20] = 
    {
        &rightTrace, &backTrace, 
        &floorTrace, &frontTrace, &leftTrace, 
        &ball,
    };
    int trace_count = 6;

    SamplerData data = (SamplerData){
        .sample_count = 4,
        .set_count = 4,
        .used_count = 0,
        .init_index = 0,
        .samples = NULL,
        .shuffled_indices = NULL
    };

    PinholeData p_data = {.d = 100.0};

    FishEyeData f_data = 
    {
        .height = canvas->height,
        .width = canvas->width,
        .pixel_size = 1,
        .max_psi = 360
    };

    SphericalData s_data = 
    {
        .height = canvas->height,
        .width = canvas->width,
        .pixel_size = 1,
        .max_lambda = 90,
        .max_psi = 90
    };

    CameraData cam_data = (CameraData)
    {
        .eye = {.x =  200, .y = -200, .z = 120},
        .look_at = {.x = 180, .y = -200, .z = 0},
        .up = {.x = 0, .y = 1, .z = 0},
        .extra = &p_data,
    };

    Camera pinhole_cam = PINHOLE(cam_data);
    Camera fisheye_cam = FISH_EYE(cam_data);
    Camera sherical_cam = SPHERICAL_CAM(cam_data);
    (void)p_data;
    (void)pinhole_cam;
    (void)f_data;
    (void)fisheye_cam;
    (void)s_data;
    (void)sherical_cam;

    Sampler jittered = JITTERED_SAMPLER(data);
    SceneData scene_data = {
        .canvas = canvas, 
        .default_colour = BLACK,
        .sampler = &jittered,
        .pixelSize = 1,
        .ray = &r,
        .traceables = &traceables[0], 
        .traceable_count = trace_count,
        .cam = &pinhole_cam,
        .optimized = 1,
    };

    Scene scene = {
        .scene_data = &scene_data,
        .ray_trace = perspective_tracer
    };

    scene.ray_trace(&scene_data);
}
