#include <stdio.h>
//#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "./stb_image.h"
#include "./stb_image_write.h"
#include "./shapes.h"
#include "./newton_fractals.h"
#include "logic_util.h"
#include "math/vector3d.h"
#include "processing.h"
#include "colours.h"
#include "raytracer/scenes/sample_scene.h"

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
Canvas canvas = { BG, "default", pixels, WIDTH, HEIGHT };
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
void read_to_canvas(Canvas* c, char* path)
{
    int w, h, channels;
    stbi_uc* data = stbi_load(path, &w, &h, &channels, STBI_rgb_alpha);
    u32* colour = (u32*)malloc(w * h * sizeof(u32));
    memcpy(colour, data, w * h * sizeof(u32));
    stbi_image_free(data);

    c->pixels = colour;
    c->width = w;
    c->height = h;
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
    Canvas fractalCanvas = {BG, "frac", fractalTexture.data, 200, 200};

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
    rect_scene(canvas);
    save(canvas, JPG, "./imgs/raytracer/rect");
}


u32 checkConcave(u32* points, u32 count)
{
    float oldCross = 0.0f;
    for (int i = 0; i < 2*count; i+=2)
    {
        Vector3d firstSide = {points[i], points[i + 1], 0};
        Vector3d secondSide = {points[(i+2) % (2*count)], points[(i + 3) % (2*count)], 0};
        float crossValue = cross(&firstSide, &secondSide).z;

        if (!(oldCross == 0.0f || oldCross * crossValue > 0))
        {
            return i + 2;
        }
        oldCross = crossValue;
    }
    return count;
}

void drawMulti(Canvas* canvas, u32* points, u32 size,  u32 count, u32 col, u32 offset)
{
    for (int i = 0; i < 2*count; i+=2)
    {
        u32 startInd = (i + offset) % (2 * size);
        line(canvas, points[startInd], points[startInd + 1], 
                points[(startInd+2) % (2*size)], points[(startInd+3) % (2 * size)], col);
    }
}
void concaveCheckShowcase(Canvas* canvas)
{
    u32 centx = 300;
    u32 centy = 300;
    u32 offsetx = 100;
    u32 offsety = 100;
    #define SIZE (8)
      u32 tri[SIZE] = 
      {
           centx - offsetx, centy,
           centx + offsetx, centy,
           centx + offsetx, centy - offsety,
           centx - offsetx, centy - offsety,
      };
    u32 concavePoint = checkConcave(&tri[0], SIZE / 2);
    u32 col = CYAN;

    drawMulti(canvas, &tri[0], SIZE / 2, (concavePoint + 2) / 2, col, 0 );
    col = MAGENTA;
    drawMulti(canvas, &tri[0], SIZE / 2, (SIZE - concavePoint - 2) / 2, col, concavePoint);

    centy = 400;
      u32 tri2[SIZE] = 
      {
           centx - offsetx, centy,
           centx, centy - offsety,
           centx + offsetx, centy,
           centx, centy - 2*offsety,
      };
    concavePoint = checkConcave(&tri2[0], SIZE / 2);
    col = CYAN;

    drawMulti(canvas, &tri2[0], SIZE / 2, (concavePoint + 2) / 2, col, 0);
    col = MAGENTA;
    drawMulti(canvas, &tri2[0], SIZE / 2, (SIZE - concavePoint) / 2, col, concavePoint);
    
}
void circleShowCase(Canvas* c)
{
    fill_circle(c, WIDTH/2, HEIGHT/2, 20, MAGENTA);
}

void processing_showcase(Canvas* c)
{
    u32 r = 20;
    fill(c, BG);
    fill_circle(c, WIDTH/2, HEIGHT/2, r, RED);
    fill_circle(c, WIDTH/2, HEIGHT/2 + 2*r, r, BLUE);
    save(c, JPG, "./imgs/processing/org");
    mult_contrast(c, 0.2); 
    save(c, JPG, "./imgs/processing/contr");
    mult_contrast(c, 1.0f/0.2);
    save(c, JPG, "./imgs/processing/recontr");
    inc_brightness(c, 10);
    save(c, JPG, "./imgs/processing/firstInc");
    inc_brightness(c, 30);
    save(c, JPG, "./imgs/processing/secondInc");
    inc_brightness(c, -40);
    save(c, JPG, "./imgs/processing/restoreBright");
    invert(c); 
    save(c, JPG, "./imgs/processing/invert");
    invert(c); 
    save(c, JPG, "./imgs/processing/restoreInvert");
    filter(c, ChannelBlue);
    save(c, JPG, "./imgs/processing/blueFilter");
    fill_circle(c, WIDTH/2, HEIGHT/2 + 2*r, r, BLUE);
    filter(c, ChannelRed);
    save(c, JPG, "./imgs/processing/redFilter");
    fill_circle(c, WIDTH/2, HEIGHT/2, r, RED);
    clamp_channel(c, ChannelBlue, 127, 255);
    save(c, JPG, "./imgs/processing/changeMinBlue");
    clamp_channel(c, ChannelRed, 127, 255);
    save(c, JPG, "./imgs/processing/changeMinRed");

    // Restore
    fill(c, BG);
    fill_circle(c, WIDTH/2, HEIGHT/2, r, PALE_RED);
    fill_circle(c, WIDTH/2, HEIGHT/2 + 2*r, r, PALE_GREEN);
    inc_channel(c, ChannelRed, 127);
    save(c, JPG, "./imgs/processing/reddishStuff");
    inc_channel(c, ChannelRed, -127);
    inc_channel(c, ChannelBlue, 28);
    save(c, JPG, "./imgs/processing/bluishStuff");

    fill(c, BG);
    fill_circle(c, WIDTH/2, HEIGHT/2, r, PALE_RED);
    fill_circle(c, WIDTH/2, HEIGHT/2 + 2*r, r, PALE_GREEN);
    grey_scale(c);
    save(c, JPG, "./imgs/processing/grey_scale");
}

void filters_showcase(Canvas* c)
{
    u32 r = 20;
    fill(c, BG);
    fill_circle(c, WIDTH/2, HEIGHT/2, r, RED);
    fill_circle(c, WIDTH/2, HEIGHT/2 + 2*r, r, BLUE);
    save(c, JPG, "./imgs/filters/org");
    i32 linear[3] = {1, 1, 1};
    general_linear_separated_filter(c, linear, 3, linear, 3);
    save(c, JPG, "./imgs/filters/linear_1");

    fill(c, BG);
    fill_circle(c, WIDTH/2, HEIGHT/2, r, RED);
    fill_circle(c, WIDTH/2, HEIGHT/2 + 2*r, r, BLUE);
    box_filter(c, 5);
    save(c, JPG, "./imgs/filters/linear_2");

    fill(c, BG);
    fill_circle(c, WIDTH/2, HEIGHT/2, r, RED);
    fill_circle(c, WIDTH/2, HEIGHT/2 + 2*r, r, BLUE);
    binomial_filter(c, 1);
    save(c, JPG, "./imgs/filters/binomial_1");

    fill(c, BG);
    fill_circle(c, WIDTH/2, HEIGHT/2, r, RED);
    fill_circle(c, WIDTH/2, HEIGHT/2 + 2*r, r, BLUE);
    binomial_filter(c, 3);
    save(c, JPG, "./imgs/filters/binomial_2");

    fill(c, BG);
    fill_circle(c, WIDTH/2, HEIGHT/2, r, RED);
    fill_circle(c, WIDTH/2, HEIGHT/2 + 2*r, r, BLUE);
    min_filter(c, 3);
    save(c, JPG, "./imgs/filters/min");

    fill(c, BG);
    fill_circle(c, WIDTH/2, HEIGHT/2, r, RED);
    fill_circle(c, WIDTH/2, HEIGHT/2 + 2*r, r, BLUE);
    max_filter(c, 3);
    save(c, JPG, "./imgs/filters/max");

    fill(c, BG);
    fill_circle(c, WIDTH/2, HEIGHT/2, r, RED);
    fill_circle(c, WIDTH/2, HEIGHT/2 + 2*r, r, BLUE);
    jitter_filter(c, 10);
    save(c, JPG, "./imgs/filters/jitter");

    fill(c, BG);
    fill_circle(c, WIDTH/2, HEIGHT/2, r, RED);
    fill_circle(c, WIDTH/2, HEIGHT/2 + 2*r, r, BLUE);
    sobel_filter(c);
    save(c, JPG, "./imgs/filters/sobel2");
    (void)r;

    
    read_to_canvas(c, "./assets/levi.jpg");
    jitter_filter(c, 10);
    save(c, JPG, "./imgs/filters/jitter2");
    free(c->pixels);

    read_to_canvas(c, "./assets/levi.jpg");
    sobel_filter(c);
    save(c, JPG, "./imgs/filters/sobel1");
    

    
    read_to_canvas(c, "./assets/levi.jpg");
    canny_filter(c, 0x02, 0x20);
    save(c, JPG, "./imgs/filters/canny");
}

int main()
{
    srand(time(NULL));
    fill(&canvas, BG);
    //filters_showcase(&canvas);
    raytrace(&canvas);
}
