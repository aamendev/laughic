#include "sample_scene.h"
#include "../lights/point_light.h"
#include "../lights/ambient_light.h"
#include "../../raytracer/brdf/lambertain.h"
#include "../../raytracer/brdf/glossy_specular.h"

void rect_scene(Canvas* canvas)
{

    Vector3d rayDirection = {.x = 0, .y = 0, .z = -1};
    Vector3d initPosition = {.x = 0, .y = 0, .z = 100};
    Ray r = {.direction = rayDirection, .base = initPosition};

    Sphere s = {.center = {.x = 50, .y = -30, .z = 25}, .r = 20.0f};
    AABB floor = 
    {
        .min_coord = {.x = -200, .y = 101, .z = -200},
        .max_coord = {.x = 200, .y = 100, .z = 200},
    };
    AABB front = 
    {
        .min_coord = {.x = 0, .y = 0, .z = 50}, 
        .max_coord = {.x = 100, .y = 100, .z = 51}
    };
    AABB right = 
    {
        .min_coord = {.x = 100, .y = 0, .z = 0},
        .max_coord = {.x = 101, .y = 100, .z = 50}, 
    };
    AABB back = 
    {
        .min_coord = {.x = 0, .y = 0, .z = 0}, 
        .max_coord = {.x = 100, .y = 100, .z = 1}
    };

    AABB left = 
    {
        .min_coord = {.x = 0, .y = 0, .z = 0},
        .max_coord = {.x = 1, .y = 100, .z = 50}, 
    };

    LambertainData front_diffuse_data = 
    {
        .kd = 0.8f,
        .cd = YELLOW
    };
    BRDF front_diffuse = LAMBERTAIN_BRDF(&front_diffuse_data, "front_diffuse"); 
    GlossySpecularData front_specular_data = 
    {
        .ks = 0.3f,
        .cs = YELLOW,
        .exp = 10,
    };
    BRDF front_specular = GLOSSY_SPECULAR_BRDF(&front_specular_data, "front_specular"); 

    Traceable frontTrace = AABB_TRACE(&front, YELLOW, "bf");
    frontTrace.specular = front_specular;
    frontTrace.diffuse = front_diffuse;

    LambertainData right_diffuse_data = 
    {
        .kd = 0.5f,
        .cd = RED
    };
    BRDF right_diffuse = LAMBERTAIN_BRDF(&right_diffuse_data, "right_diffuse"); 
    GlossySpecularData right_specular_data = 
    {
        .ks = 0.9f,
        .cs = RED,
        .exp = 10,
    };
    BRDF right_specular = GLOSSY_SPECULAR_BRDF(&right_specular_data, "right_specular"); 

    Traceable rightTrace = AABB_TRACE(&right, RED, "br");
    rightTrace.specular = right_specular;
    rightTrace.diffuse = right_diffuse;

    LambertainData left_diffuse_data = 
    {
        .kd = 0.6f,
        .cd = BLUE,
    };
    BRDF left_diffuse = LAMBERTAIN_BRDF(&left_diffuse_data, "left_diffuse"); 
    GlossySpecularData left_specular_data = 
    {
        .ks = 0.7f,
        .cs = BLUE,
        .exp = 10,
    };
    BRDF left_specular = GLOSSY_SPECULAR_BRDF(&left_specular_data, "left_specular"); 

    Traceable leftTrace = AABB_TRACE(&left, BLUE, "br");
    leftTrace.specular = left_specular;
    leftTrace.diffuse = left_diffuse;

    LambertainData back_diffuse_data = 
    {
        .kd = 0.5f,
        .cd = GREEN
    };
    BRDF back_diffuse = LAMBERTAIN_BRDF(&back_diffuse_data, "back_diffuse"); 
    GlossySpecularData back_specular_data = 
    {
        .ks = 0.5f,
        .cs = GREEN,
        .exp = 10,
    };
    BRDF back_specular = GLOSSY_SPECULAR_BRDF(&back_specular_data, "back_specular"); 

    Traceable backTrace = AABB_TRACE(&back, GREEN, "br");
    backTrace.specular = back_specular;
    backTrace.diffuse = back_diffuse;

    LambertainData floor_diffuse_data = 
    {
        .kd = 0.9f,
        .cd = WHITE
    };
    BRDF floor_diffuse = LAMBERTAIN_BRDF(&floor_diffuse_data, "floor_diffuse"); 
    GlossySpecularData floor_specular_data = 
    {
        .ks = 0.0f,
        .cs = WHITE,
        .exp = 1,
    };
    BRDF floor_specular = GLOSSY_SPECULAR_BRDF(&floor_specular_data, "floor_specular"); 

    Traceable floorTrace = AABB_TRACE(&floor, WHITE, "br");
    floorTrace.specular = floor_specular;
    floorTrace.diffuse = floor_diffuse;

    LambertainData ball_diffuse_data = 
    {
        .kd = 0.7f,
        .cd = CYAN
    };
    BRDF ball_diffuse = LAMBERTAIN_BRDF(&ball_diffuse_data, "ball_diffuse"); 
    GlossySpecularData ball_specular_data = 
    {
        .ks = 0.7f,
        .cs = CYAN,
        .exp = 20,
    };
    BRDF ball_specular = GLOSSY_SPECULAR_BRDF(&ball_specular_data, "ball_specular"); 

    Traceable ballTrace = SPHERE_TRACE(&s, CYAN, "br");
    ballTrace.specular = ball_specular;
    ballTrace.diffuse = ball_diffuse;

    Traceable* traceables[20] = 
    {
        &rightTrace, &backTrace, 
        &floorTrace, &frontTrace, &leftTrace, 
        &ballTrace,
    };
    int trace_count = 6;
   // (void)ball;
    PointLight pl = 
    {
        .colour = WHITE,
        .ls = 1.0e5f,
        .pos = {100, -350, 50}
    };

    AmbientLight amb = 
    {
        .colour = WHITE,
        .ls = 0.0f
    };
    Light first_pl = POINT_LIGHT(&pl, "fpl");
    Light first_amb = AMBIENT_LIGHT(&amb, "famb");
    Light lights[20] = {first_pl, first_amb};
    int light_count = 1;

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
    MaterialData mat_data = 
    {
        .ambient_light = NULL,
        .lights = &lights[0],
        .light_count = light_count
    };

    Scene scene = {
        .scene_data = &scene_data,
        .ray_trace = ligh_tracer
    };

    scene.ray_trace(&scene_data, &mat_data);
}
