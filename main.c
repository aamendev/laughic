#include <stdio.h>
#include "./bspline_modify_util.h"
//#include <errno.h>
#include <stdint.h>
#include <time.h>
#include "./shapes.h"
#include "./newton_fractals.h"
#include "logic_util.h"
#include "processing/processing_tests.h"
#include "spline_samples.h"
#include "math/vector3d.h"
#include "processing/processing.h"
#include "colours.h"
#include "raytracer/scenes/sample_scene.h"
#include "./experiments/noise_texture.h"
#include "core/core_tests.h"

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
    //test_scene(canvas);
    save_to_img(canvas, JPG, "./imgs/raytracer/new_light");
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
    save_to_img(c, JPG, "./imgs/processing/org");
    mult_contrast(c, 0.2); 
    save_to_img(c, JPG, "./imgs/processing/contr");
    mult_contrast(c, 1.0f/0.2);
    save_to_img(c, JPG, "./imgs/processing/recontr");
    inc_brightness(c, 10);
    save_to_img(c, JPG, "./imgs/processing/firstInc");
    inc_brightness(c, 30);
    save_to_img(c, JPG, "./imgs/processing/secondInc");
    inc_brightness(c, -40);
    save_to_img(c, JPG, "./imgs/processing/restoreBright");
    invert(c); 
    save_to_img(c, JPG, "./imgs/processing/invert");
    invert(c); 
    save_to_img(c, JPG, "./imgs/processing/restoreInvert");
    filter(c, ChannelBlue);
    save_to_img(c, JPG, "./imgs/processing/blueFilter");
    fill_circle(c, WIDTH/2, HEIGHT/2 + 2*r, r, BLUE);
    filter(c, ChannelRed);
    save_to_img(c, JPG, "./imgs/processing/redFilter");
    fill_circle(c, WIDTH/2, HEIGHT/2, r, RED);
    clamp_channel(c, ChannelBlue, 127, 255);
    save_to_img(c, JPG, "./imgs/processing/changeMinBlue");
    clamp_channel(c, ChannelRed, 127, 255);
    save_to_img(c, JPG, "./imgs/processing/changeMinRed");

    // Restore
    fill(c, BG);
    fill_circle(c, WIDTH/2, HEIGHT/2, r, PALE_RED);
    fill_circle(c, WIDTH/2, HEIGHT/2 + 2*r, r, PALE_GREEN);
    inc_channel(c, ChannelRed, 127);
    save_to_img(c, JPG, "./imgs/processing/reddishStuff");
    inc_channel(c, ChannelRed, -127);
    inc_channel(c, ChannelBlue, 28);
    save_to_img(c, JPG, "./imgs/processing/bluishStuff");

    fill(c, BG);
    fill_circle(c, WIDTH/2, HEIGHT/2, r, PALE_RED);
    fill_circle(c, WIDTH/2, HEIGHT/2 + 2*r, r, PALE_GREEN);
    grey_scale(c);
    save_to_img(c, JPG, "./imgs/processing/grey_scale");


}

void filters_showcase(Canvas* c)
{
    u32 r = 20;
    fill(c, BG);
    fill_circle(c, WIDTH/2, HEIGHT/2, r, RED);
    fill_circle(c, WIDTH/2, HEIGHT/2 + 2*r, r, BLUE);
    save_to_img(c, JPG, "./imgs/filters/org");
    i32 linear[3] = {1, 1, 1};
    general_linear_separated_filter(c, linear, 3, linear, 3);
    save_to_img(c, JPG, "./imgs/filters/linear_1");

    fill(c, BG);
    fill_circle(c, WIDTH/2, HEIGHT/2, r, RED);
    fill_circle(c, WIDTH/2, HEIGHT/2 + 2*r, r, BLUE);
    box_filter(c, 5);
    save_to_img(c, JPG, "./imgs/filters/linear_2");

    fill(c, BG);
    fill_circle(c, WIDTH/2, HEIGHT/2, r, RED);
    fill_circle(c, WIDTH/2, HEIGHT/2 + 2*r, r, BLUE);
    binomial_filter(c, 1);
    save_to_img(c, JPG, "./imgs/filters/binomial_1");

    fill(c, BG);
    fill_circle(c, WIDTH/2, HEIGHT/2, r, RED);
    fill_circle(c, WIDTH/2, HEIGHT/2 + 2*r, r, BLUE);
    binomial_filter(c, 3);
    save_to_img(c, JPG, "./imgs/filters/binomial_2");

    fill(c, BG);
    fill_circle(c, WIDTH/2, HEIGHT/2, r, RED);
    fill_circle(c, WIDTH/2, HEIGHT/2 + 2*r, r, BLUE);
    min_filter(c, 3);
    save_to_img(c, JPG, "./imgs/filters/min");

    fill(c, BG);
    fill_circle(c, WIDTH/2, HEIGHT/2, r, RED);
    fill_circle(c, WIDTH/2, HEIGHT/2 + 2*r, r, BLUE);
    max_filter(c, 3);
    save_to_img(c, JPG, "./imgs/filters/max");

    fill(c, BG);
    fill_circle(c, WIDTH/2, HEIGHT/2, r, RED);
    fill_circle(c, WIDTH/2, HEIGHT/2 + 2*r, r, BLUE);
    jitter_filter(c, 10);
    save_to_img(c, JPG, "./imgs/filters/jitter");

    fill(c, BG);
    fill_circle(c, WIDTH/2, HEIGHT/2, r, RED);
    fill_circle(c, WIDTH/2, HEIGHT/2 + 2*r, r, BLUE);
    sobel_filter(c);
    save_to_img(c, JPG, "./imgs/filters/sobel2");
    (void)r;

    
    read_to_canvas(c, "./assets/levi.jpg");
    jitter_filter(c, 10);
    save_to_img(c, JPG, "./imgs/filters/jitter2");
    free(c->pixels);

    read_to_canvas(c, "./assets/levi.jpg");
    sobel_filter(c);
    save_to_img(c, JPG, "./imgs/filters/sobel1");
    

    
    read_to_canvas(c, "./assets/levi.jpg");
    canny_filter(c, 0x02, 0x20);
    save_to_img(c, JPG, "./imgs/filters/canny");


}

void new_processing(Canvas* c)
{

    fill(c,BLACK);
    fill_rect(c, c->width/2, c->height/2, 50, 50, BLUE);
    rect(c, c->width/2, c->height/2, 50, 50, WHITE);
    harris_filter(c, 0.07f, 2e4);
    save_to_img(c, JPG, "./imgs/filters/harris");

    read_to_canvas(c, "./assets/levi.jpg");
    translate(c, -c->width/2, c->height/2);
    save_to_img(c, JPG, "./imgs/processing/translate");
}
void npr_curve(Canvas* c)
{
    int spline_offset_x = c->width / 8;
    int spline_offset_y = c->height / 8;
    float xcoffs[6] = 
    {
        c->width / 4, 
        c->width / 2 - spline_offset_x,
        c->width / 2 + spline_offset_x,
        (3 * c->width) / 4,
        c->width / 2,
        c->width / 2 - 2 * spline_offset_x,
    };

    float ycoffs[6] = 
    {
        c->height / 2, 
        c->height / 4 + spline_offset_y,
        c->height / 2, 
        c->height / 4 + 2 * spline_offset_y,
        c->height / 4 + spline_offset_y,
        c->height / 4 + 2 * spline_offset_y,
    };
    float knots[14] = {
        0,
        0.111f,
        0.222f,
        0.333f,
        0.444f,
        0.556f,
        0.667f,
        0.778,
        0.889,
        1.0f,
    };
    BSpline test_spline = 
    {
        .x_coeffs = xcoffs,
        .y_coeffs = ycoffs,
        .coeffs_count= 6,
        .seg_count = 1e2,
        .knots = knots,
        .knot_count = 10,
        .order = 4,
    };

    SimpleBrush sb = 
    {
        .colour = BLUE,
        .r = 1
    };
    Channel curr_channel = ChannelAlpha;
    bspline_modify(c, &test_spline, &sb, &curr_channel, bspline_channel_modify);

    for (int i = 0; i < test_spline.coeffs_count; i++)
    {
       test_spline.y_coeffs[i] += 100; 
    }

    RadiusOpt rad_opt =
    {
        .min_r = 1,
        .max_r = 5
    };

    bspline_modify(c, &test_spline, &sb, &rad_opt, bspline_width_modify);

    for (int i = 0; i < test_spline.coeffs_count; i++)
    {
       test_spline.y_coeffs[i] += 100; 
    }
    sb.colour = GREEN;

    RadiusColOpt rad_col_opt = 
    {
        rad_opt,
        ChannelRed
    };
    //bspline_channel_smooth(c, &test_spline, GREEN, ChannelRed);
    bspline_modify(c, &test_spline, &sb, &rad_col_opt, bspline_channel_width_modify);

    sb.colour = RED;
    sb.r = 1;
    SimpleWiggleOpt s_wiggle_opt = 
    {
        .min_wiggle_x = 0,
        .max_wiggle_x = 1,
        .min_wiggle_y = 0,
        .max_wiggle_y = 5,
        .prob = 20,
    };

    for (int i = 0; i < test_spline.coeffs_count; i++)
    {
       test_spline.y_coeffs[i] += 100; 
    }
    bspline_modify(c, &test_spline, &sb, &s_wiggle_opt, bspline_simple_wiggle_modify);

    for (int i = 0; i < test_spline.coeffs_count; i++)
    {
       test_spline.y_coeffs[i] -= 500; 
    }

    SimpleRangeWiggleOpt s_range_wiggle_opt = 
    {
        .min_wiggle_x = 0,
        .max_wiggle_x = 1,
        .min_wiggle_y = 0,
        .max_wiggle_y = 5,
        .prob = 5,
        .min_u = 0.45f,
        .max_u = 0.55f,
    };
    bspline_modify(c, &test_spline, &sb, &s_range_wiggle_opt, bspline_simple_range_wiggle_modify);

    for (int i = 0; i < test_spline.coeffs_count; i++)
    {
       test_spline.y_coeffs[i] += 100; 
    }

    bspline_lod_test(c, &test_spline, &sb, 20);

    for (int i = 0; i < test_spline.coeffs_count; i++)
    {
       test_spline.y_coeffs[i] += 100; 
    }

    bspline_circ_test(c, &test_spline, &sb, 40);

    for (int i = 0; i < test_spline.coeffs_count; i++)
    {
       test_spline.x_coeffs[i] -= 100; 
    }
    //bspline_lina(c, &sb, test_spline.x_coeffs[0], test_spline.y_coeffs[0]);
    save_to_img(c, JPG, "./imgs/npr/curve2");
}
void npr_processing(Canvas* c)
{
    read_to_canvas(c, "./assets/levi.jpg");
    default_ordered_dithering(c);
    save_to_img(c, JPG, "./imgs/npr/dither");
    read_to_canvas(c, "./assets/levi.jpg");
    default_floyd_steinberg(c);
    save_to_img(c, JPG, "./imgs/npr/floyd_stienberg");

    processing_all_tests(c);
}
void npr_test(Canvas* c)
{
    read_to_canvas(c, "./assets/ramp");
    //default_grey_scale_ordered_dithering(c);
    default_grey_scale_floyd_steinberg(c);
    save_to_img(c, JPG, "./imgs/npr/dither");
}
void intensity_ramp(Canvas* c)
{
    u8 curr_int;
    u8 comps[4];
    u32 current_col = 0;
    fill(c, BLACK);
    for (int i = 0; i < c->width; i++)
    {
        current_col = 0;
        curr_int = ((float)i / c->width) * 0xff;
        for (int j = 0; j < 3; j++)
        {
            comps[j] = curr_int;
        }
        comps[3] = 0xff;
        pack(comps, &current_col);
        line(c, i, 0, i, c->height - 1, current_col);
    }
    save_to_img(c, JPG, "./assets/ramp");
}
void run_exp(Canvas* c)
{
    init_noise();
    float* data = malloc(c->width * c->height * sizeof(float));
    //warp(1.8f, 1.2f, 8, 0.7f, data, c->width, c->height);
    worley(0, data, c->width, c->height);
    //fbm(1.8f, 1.2f, 12, data, c->width, c->height);
    u8 comps[4] = {0,0,0,255};
    for (int i = 0; i < c->width * c->height; i++)
    {
        f32 currData = fabs(data[i]) * (fabs(data[i]) <= 1.0f) + 
            1.0f * (fabs(data[i]) > 1.0f);
        comps[0] = currData * 255; 
        comps[1] = currData * 255; 
        comps[2] = currData * 255; 
        c->pixels[i] = 0;
        pack(comps, &c->pixels[i]);
    }
    free(data);
    save_to_img(c, JPG, "./imgs/experiments/noise");
}

void curve_exp(Canvas* c)
{
    int offsetx = -c->width/4;
    int offsety = 0 * c->height/4;
    ParametricCubic2D pc0 = 
    {
        {
            .x = c->width / 2 + offsetx,
            .y = c->height / 2 + offsety,
            },
        .ax = 100.0f,
        .bx = 100.0f,
        .cx = 100.0f,
        .dx = 100.0f,
        .ay = 0.0f,
        .by = 0.0f,
        .cy = -100.0f,
        .dy = -100.0f,
        .seg_num = 1e5,
    };
    parametric_cubic(c, &pc0, RED);
    pc0.cy *= -1;
    pc0.dy *= -1;
    parametric_cubic(c, &pc0, YELLOW);

    int spline_offset_x = c->width / 8;
    int spline_offset_y = c->height / 8;
    float xcoffs[6] = 
    {
        c->width / 4, 
        c->width / 2 - spline_offset_x,
        c->width / 2 + spline_offset_x,
        (3 * c->width) / 4,
        c->width / 2,
        c->width / 2 - 2 * spline_offset_x,
    };

    float ycoffs[6] = 
    {
        c->height / 2, 
        c->height / 4 + spline_offset_y,
        c->height / 2, 
        c->height / 4 + 2 * spline_offset_y,
        c->height / 4 + spline_offset_y,
        c->height / 4 + 2 * spline_offset_y,
    };
    float knots[14] = {
        0,
        0.111f,
        0.222f,
        0.333f,
        0.444f,
        0.556f,
        0.667f,
        0.778,
        0.889,
        1.0f,
    };
    BSpline test_spline = 
    {
        .x_coeffs = xcoffs,
        .y_coeffs = ycoffs,
        .coeffs_count= 6,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 10,
        .order = 4,
    };

    SimpleBrush sb = 
    {
        .colour = BLUE,
        .r = 1,
    };
    
    bspline(c, &test_spline, &sb);

    save_to_img(c, JPG, "./imgs/curves/curve");
}

void npr_path_style_sample(Canvas* c)
{
    SimpleBrush sb = 
    {
        .colour = BLACK,
        .r = 1,
    };
    int x_off = 15;
    int y_off = 30;

    bspline_face_outline(c, &sb, c->width / 4, c->height / 4);
    bspline_l_eye(
            c, 
            &sb, 
            c->width / 4 + x_off,
            c->height / 4 + y_off);

    x_off = 105;
    y_off = 30;
    bspline_r_eye(
            c, 
            &sb, 
            c->width / 4 + x_off,
            c->height / 4 + y_off);

    x_off = 50;
    y_off = 80;
    bspline_nose(
            c, 
            &sb, 
            c->width / 4 + x_off,
            c->height / 4 + y_off);

    x_off = 30;
    y_off = 120;
    bspline_smile(
            c, 
            &sb, 
            c->width / 4 + x_off,
            c->height / 4 + y_off);


    SimpleWiggleOpt s_wiggle_opt = 
    {
        .min_wiggle_x = 0,
        .max_wiggle_x = 1,
        .min_wiggle_y = 0,
        .max_wiggle_y = 10,
        .prob = 20,
    };

    RadiusOpt r_opt = 
    {
        .min_r = 1,
        .max_r = 3
    };

    sb.r = 1;
    y_off = 0;
    x_off = c->width / 4;
    bspline_wiggle_face_outline(
            c, &sb, 
            c->width/4 + x_off,
            c->height / 4 + y_off,
            &s_wiggle_opt, bspline_simple_wiggle_modify);
            

    y_off += 30;
    x_off += 15;

    bspline_width_l_eye(
            c, &sb, 
            c->width/4 + x_off,
            c->height / 4 + y_off,
            &r_opt, bspline_width_modify);

    y_off += 0;
    x_off += 85;

    bspline_width_r_eye(
            c, &sb, 
            c->width/4 + x_off,
            c->height / 4 + y_off,
            &r_opt, bspline_width_modify);

    y_off += 50;
    x_off -= 50;

    bspline_width_nose(
            c, &sb, 
            c->width/4 + x_off,
            c->height / 4 + y_off,
            &r_opt, bspline_width_modify);

    y_off += 50;
    x_off -= 20;

    bspline_width_smile(
            c, &sb, 
            c->width/4 + x_off,
            c->height / 4 + y_off,
            &r_opt, bspline_mid_width_modify);
    (void) s_wiggle_opt;

    save_to_img(c, JPG, "./imgs/npr/ps1");
}
void bounded(Canvas* c)
{
    Circle circ = {.cx = c->width/2, .cy = c->height/2, .r = 20};
    ParametricLine pl = {
        .x0 = circ.cx - circ.r - 2, .y0 = circ.cy,
        .dx = 2,
        .dy = -1,
        .t = 1,
    };
    bounded_circle(c, &circ, &pl, RED);
    save_to_img(c, JPG, "./imgs/npr/bounded_circle");
}
int main()
{
    srand(time(NULL));
    //all_tests();

    //new_processing(&canvas);
    //fill(&canvas, BG);
    //filters_showcase(&canvas);
    //raytrace(&canvas);
    //intensity_ramp(&canvas);
    npr_processing(&canvas);
    //curve_exp(&canvas);
    //npr_path_style_sample(&canvas);
  //  bounded(&canvas);
    //npr_curve(&canvas);
    //run_exp(&canvas);
}
