#include <stdio.h>
//#include <errno.h>
#include <stdint.h>
#include <string.h>
#include "./stb_image.h"
#include "./stb_image_write.h"
#include "./shapes.h"
#include "./newton_fractals.h"
#include "cameras/camera.h"
#include "cameras/fish_eye.h"
#include "cameras/pinhole.h"
#include "cameras/spherical.h"
#include "logic_util.h"
#include "math/ray.h"
#include "math/sphere.h"
#include "math/traceable.h"
#include "math/vector3d.h"
#include "managers/scene.h"
#include "samplers/jittered.h"
#include "samplers/sampler.h"
#define WIDTH 800
#define HEIGHT 800

#define BG 0xff382209
#define WHITE 0xffffffff
#define BLACK 0xff000000
#define CYAN 0xffffff00
#define MAGENTA 0xffff00ff

typedef enum
{
    POINT,
    LINE_LIST,
    LINE_STRIP,
    TRIANGLE_DRAW,
    TRIANGLE_STRIP,
    TRIANGLE_FAN,
    DRAW_MODE
} DrawMode;

typedef enum
{
    SINGLE,
    BARY,
    TEXTURE,
    COLOUR_MODE
} ColourMode;

u32 pixels[WIDTH * HEIGHT];
Canvas canvas = { pixels, WIDTH, HEIGHT };
void read_texture(Texture* tex)
{
    int w, h, channels;
    stbi_uc* data = stbi_load(tex->path, &w, &h, &channels, STBI_rgb_alpha);
    u32* colour = (u32*)malloc(w * h * sizeof(u32));
    memcpy(colour, data, w * h * sizeof(u32));
    stbi_image_free(data);

    tex->data = colour;
    tex->width = w;
    tex->height = h;
}

/* Unused in show case;
 * Need to Handle Bary and Texture Modes
 */
void draw(Canvas* canvas, u32 colour, void* data, u32 count, u32 stride, DrawMode drawMode, ColourMode colourMode)
{
    switch (colourMode)
    {
        case SINGLE:
            {
                switch (drawMode)
                {
                    case POINT:
                        {
                            u32* data2 = data;
                            for (int i = 0; i < (count - 1) * stride + 1; i+=stride)
                            {
                                point(canvas, data2[i], data2[i + 1], colour);
                            }
                        }
                        break;
                    case LINE_LIST:
                        { u32* data2 = data;
                            for (int i = 0; i < (count - 2) * stride + 1; i += stride * 2)
                            {
                                line(canvas, data2[i], data2[i + 1], data2[i + 2], data2[i + 3], colour);
                            }
                        }
                        break;
                    case LINE_STRIP:
                        {
                            u32* data2 = data;
                            for (int i = 0; i < (count - 2) * stride + 1; i += stride)
                            {
                                line(canvas, data2[i], data2[i + 1], data2[i + 2], data2[i + 3], colour);
                            }
                        }
                        break;
                    case TRIANGLE_DRAW:
                        {
                            u32* data2 = data;
                            for (int i = 0; i < (count - 3) * stride + 1; i += stride * 3)
                            {
                                fill_triangle(canvas, data2[i], data2[i + 1], data2[i + 2], data2[i +3], data2[i + 4], data2[i + 5] , colour);
                            }
                        }
                        break;
                    case TRIANGLE_STRIP:
                        {
                            u32* data2 = data;
                            for (int i = 0; i < (count - 3) * stride + 1; i += stride)
                            {
                                fill_triangle(canvas, data2[i], data2[i + 1], data2[i + 2], data2[i + 3], data2[i + 4], data2[i + 5], colour);
                            }
                        }
                        break;
                    case TRIANGLE_FAN:
                        {
                            u32* data2 = data;
                            for (int i = 2; i < (count - 2) * stride + 1; i += stride)
                            {
                                fill_triangle(canvas, data2[0], data2[1], data2[i], data2[i + 1], data2[i + 2], data2[i + 3], colour);
                            }
                        }
                        break;
                    default:
                        printf("Invalid DrawMode");
                }
            }
            break;
        default:
            printf("Invalid ColourMode");
    }
}


int save(Canvas* canvas, Format format, char* dst)
{
    switch (format)
    {
        case PNG:
        {
            return stbi_write_png(dst, canvas->width, canvas->height, 4, canvas->pixels, WIDTH * 4);
        }
        break;
        case JPG:
        {
            return stbi_write_jpg(dst, canvas->width, canvas->height, 4, canvas->pixels, WIDTH * 4);
        }
        default:
            return -1;
    }
}
void showcase(Canvas* canvas)
{

    Texture earth = {"./assets/8k_earth_daymap.jpg", NULL, 0, 0};
    Texture uranus = {"./assets/uranus.jpg", NULL, 0, 0};
    Texture cU = {"./assets/uranus.jpg", NULL, 0, 0};
    Texture cEarth = {"./assets/8k_earth_daymap.jpg", NULL, 0, 0};
    Texture textures[4] = {earth, uranus, cU, cEarth};
    for (int i = 0; i < 4; i++)
    {
        read_texture(&textures[i]);
    }
    for (int i = 0; i < textures[2].width * textures[2].height; i++)
    {
        textures[2].data[i] = complement(textures[2].data[i]);
    }
    for (int i = 0; i < textures[3].width * textures[3].height; i++)
    {
        textures[3].data[i] = complement(textures[3].data[i]);
    }
   int data[4 * 3 * 2] =
   {
        300, 200, 0, 1000,
        300 + 100, 200, 1000, 1000,
        300 + 100, 200 - 100, 1000, 0,

        300, 200 - 100, 0, 0,
        300 + 100, 200 - 100, 1000, 0,
        300, 200, 0, 1000,
   };

   fill_rect_texture(canvas, 50, 100, 200, 100, &textures[3]);

   fill_triangle_texture(
           canvas,
           data[0], data[1],
           data[0 + 4], data[1 + 4],
           data[0 + 8], data[1 + 8],
           &textures[0],
           data[2], data[3],
           data[2 + 4], data[3 + 4],
           data[2 + 8], data[3 + 8]);
fill_triangle_texture( canvas,
           data[0], data[1],
           data[0 + 4 * 3], data[1 + 4 * 3],
           data[0 + 4 * 4], data[1 + 4 * 4],
           &textures[0],
           data[2], data[3],
           data[2 + 4 * 3], data[3 + 4 * 3],
           data[2 + 4 * 4], data[3 + 4 * 4]);

   fill_triangle_bary(
           canvas,
           500, 200,
           500 + 200, 200,
           500 + 100, 100,
           0xFF0000FF, 0x1100FF00, 0x22FF0000
           );

   fill_circle_texture(canvas, 100, 300, 50, &textures[1]);
   fill_ellipse(canvas, 250, 300, 30, 70, 0x47FFFF00);
   fill_ellipse(canvas, 250, 300, 70, 30, 0x47FFFF00);
   fill_circle_texture(canvas, 100, 450, 50, &textures[2]);

   rect(canvas, 400, 250, 200, 100, 0xFF772364);

    for (int i = 0; i < 2; i++)
    {
        free(textures[i].data);
    }
}
void fractal_showcase(Canvas* canvas)
{
    Texture fractalTexture = {"", NULL, 200, 200};
    fractalTexture.data = calloc(sizeof(u32), 200 * 200);
    Canvas fractalCanvas = {fractalTexture.data, 200, 200};

    fill(&fractalCanvas, BG);
    julia(&fractalCanvas, 100, -0.79, 0.1889, BG);
    fill_rect_texture(canvas, 50, 50, 200, 200, &fractalTexture);

    fill(&fractalCanvas, BG);
    mandelbrot(&fractalCanvas, 100, BG*2);
    fill_rect_texture(canvas, 300, 50, 200, 200, &fractalTexture);

    fill(&fractalCanvas, BG);
    newton(&fractalCanvas, 100, BG);
    fill_rect_texture(canvas, 550, 50, 200, 200, &fractalTexture);

    float roots[8] = {1, 0, -1, 0, 0, 1, 0, -1};
    u32 nRoots = 4;
    fill(&fractalCanvas, BG);
    newton2(&fractalCanvas, 100, func_z4_n1, deri_z4_n1, roots, nRoots);
    fill_rect_texture(canvas, 50, 300, 200, 200, &fractalTexture);

    free(fractalTexture.data);
}

void raytrace(Canvas* canvas)
{
    Vector3d rayDirection = {.x = 0, .y = 0, .z = -1};
    Vector3d initPosition = {.x = 0, .y = 0, .z = 100};
    Ray r = {.direction = rayDirection, .base = initPosition};

    Sphere s = {.center = {.x = 0, .y = 0, .z = 0}, .r = 85.0f};
    Sphere s2 = {.center =  {.x = -20, .y = 20, .z = 0}, .r = 100.0f};

    Traceable t1 = SPHERE_TRACE(s, CYAN);
    Traceable t2 = SPHERE_TRACE(s2, MAGENTA);
    Traceable* traceables[2] = {&t1, &t2};

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
        .max_lambda = 360,
        .max_psi = 180
    };

    CameraData cam_data = (CameraData)
    {
        .eye = {.x =  100, .y =  110, .z = 130},
        .look_at = {.x = 0, .y = 0, .z = -20},
        .up = {.x = 1.2, .y = 1, .z = 0},
        .extra = &s_data,
    };
    printf("%f", *(float*)cam_data.extra);

    Camera pinhole_cam = PINHOLE(cam_data);
    Camera fisheye_cam = FISH_EYE(cam_data);
    Camera sherical_cam = SPHERICAL_CAM(cam_data);
    (void)p_data;
    (void)pinhole_cam;
    (void)f_data;
    (void)fisheye_cam;

    Sampler jittered = JITTERED_SAMPLER(data);
    SceneData scene_data = {
        .canvas = canvas, 
        .default_colour = BLACK,
        .sampler = &jittered,
        .pixelSize = 1,
        .ray = &r,
        .traceables = &traceables[0], 
        .traceable_count = 2,
        .cam = &sherical_cam
    };

    Scene scene = {
        .scene_data = &scene_data,
        .ray_trace = perspective_tracer
    };

    scene.ray_trace(&scene_data);
}

int main()
{
    fill(&canvas, BG);
    raytrace(&canvas);
    save(&canvas, JPG, "./imgs/pers_raytrace");
}
