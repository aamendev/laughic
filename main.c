#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include "./stb_image.h"
#include "./stb_image_write.h"
#include "./shapes.h"

#define WIDTH 800
#define HEIGHT 800

typedef enum
{
    POINT,
    LINE_LIST,
    LINE_STRIP,
    TRIANGLE,
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
                        {
                            u32* data2 = data;
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
                    case TRIANGLE:
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


int save(Canvas* canvas, Format format)
{
    switch (format)
    {
        case PNG:
        {
            return stbi_write_png("./imgs/pic.png", canvas->width, canvas->height, 4, canvas->pixels, WIDTH * 4);
        }
        break;
        case JPG:
        {
            return stbi_write_jpg("./imgs/showcase.jpg", canvas->width, canvas->height, 4, canvas->pixels, WIDTH * 4);
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

   fill_triangle_texture(
           canvas,
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

int main()
{
    u32 pixels[WIDTH * HEIGHT];
    Canvas canvas = { pixels, WIDTH, HEIGHT };
    fill(&canvas, 0xff382209);
    showcase(&canvas);
 //   mandelbrot(&canvas, 1000, 0x77423ab5);
    //julia(&canvas, 1000, 0xaf423ab5, -0.7269, 0.1889);
    save(&canvas, JPG);
}
