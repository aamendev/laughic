#include "processing.h"
#include "logic_util.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>

void mult_contrast(Canvas* c, float val)
{
    if (val > 255.0f) return;
    for (int i = 0; i < c->width * c->height; i++)
    {
        u8 channels[4];
        unpack(channels, &c->pixels[i]);
        c->pixels[i] = 0;
        for (int j = 0; j < 3; j++)
        {
            channels[j] =  channels[j] * val * (((u16)channels[j] * val) < 256)
                + 255 * !(((u16)channels[j] * val) < 256);
        }
        pack(channels, & c->pixels[i]);
    }
}

void inc_brightness(Canvas* c, i16 val)
{
    if (val * val > 65025) return;
    for (int i = 0; i < c->width * c->height; i++)
    {
        u8 channels[4];
        unpack(channels, &c->pixels[i]);
        c->pixels[i] = 0;
        for (int j = 0; j < 3; j++)
        {
            channels[j] =  
                (channels[j] + val) * ((u16)((u16)channels[j] + val) < 256)
                + 255 * !(((u16)((u16)channels[j] + val) < 256) || val < 0);
        }
        pack(channels, & c->pixels[i]);
    }
}

void inc_channel(Canvas* c, Channel channel, i16 val)
{
    if (val * val > 65025) return;
    for (int i = 0; i < c->width * c->height; i++)
    {
        u8 channels[4];
        unpack(channels, &c->pixels[i]);
        c->pixels[i] = 0;
        channels[channel] =  
            (channels[channel] + val) * ((u16)((u16)channels[channel] + val) < 256)
            + 255 * !((u16)((u16)channels[channel] + val) < 256 || val < 0);
        pack(channels, & c->pixels[i]);
    }
}


void invert(Canvas* c)
{
    for (int i = 0; i < c->width * c->height; i++)
    {
        c->pixels[i] = complement(c->pixels[i]);
    }
}

void filter(Canvas* c, Channel channel)
{
    filter_threshold(c, channel, 0);
}

void filter_threshold(Canvas* c, Channel channel, u8 t)
{
    for (int i = 0; i < c->width * c->height; i++)
    {
        u8 comps[4];
        unpack(comps, &c->pixels[i]);
        c->pixels[i] = c->default_colour * (comps[channel] > t) + 
            c->pixels[i] * !(comps[channel] > t);
    }
}

void clamp_channel(Canvas* c, Channel channel, u8 t0, u8 t1)
{
    for (int i = 0; i < c->width * c->height; i++)
    {
        u8 comps[4];
        unpack(comps, &c->pixels[i]);
        comps[channel] = 
            comps[channel] * (comps[channel] >= t0 && comps[channel] <= t1)
            + t0 * (comps[channel] < t0)
            + t1 * (comps[channel] > t1);
        c->pixels[i] = 0;
        pack(comps, &c->pixels[i]);
    }
}
void clamp_all(Canvas* c, u8 t0, u8 t1)
{
    for (int i = 0; i < c->width * c->height; i++)
    {
        u8 comps[4];
        unpack(comps, &c->pixels[i]);
        for (int j = 0; j < 3; j++)
        {
            comps[j] = 
                comps[j] * (comps[j] >= t0 && comps[j] <= t1)
                + t0 * (comps[j] < t0)
                + t1 * (comps[j] > t1);
        }
        c->pixels[i] = 0;
        pack(comps, &c->pixels[i]);
    }
}
void grey_scale(Canvas *c)
{
    for (int i = 0; i < c->width * c->height; i++)
    {
        u8 comps[4];
        unpack(comps, &c->pixels[i]);
        comps[0] = 0.299f * comps[0] + 0.587 * comps[1] + (0.144 * comps[2]);
        comps[1] = comps[0];
        comps[2] = comps[0];
        c->pixels[i] = 0;
        pack(comps, &c->pixels[i]);
    }
}

void general_2d_generic_operator_filter(Canvas* s, i32* h1, u32 s1, u32 val_init,
        i32 normalize_factor,
        void (*op)(u32* a, u32 b))
{
    u32 oldCanvas[s->width*s->height];
    i32 c1 = s1/2;
    u32 curr_cell = 0;
    f32 norm1 = 0;
    u8 comps[4];
    u32 vals[4] = {0, 0, 0, 0};
    int add1 = 0;
    int add2 = 0;

    if (normalize_factor == 0)
    {
    for (int i = 0; i < s1; i++)
    {
        norm1 += abs(h1[i]);
    }
    norm1 = 1.0f/norm1;
    }
    else {
    norm1 = normalize_factor;
    }

    for (int i = 0; i < s->width * s->height; i++)
    {
        oldCanvas[i] = s->pixels[i];
    }
    for (int j = 0; j < s->height; j++)
    {
        for (int i = 0; i < s->width; i++)
        {
            for (int v = 0; v < 4; v++)
            {
                vals[v] = val_init;
            }
            for (int u = -c1; u < c1 + (s1 % 2 == 1); u++)
            {
                add1 = u * (i + u > -1 && i+u < s->width);
                for (int v = -c1; v< c1 + (s1 %2 == 1); v++)
                {
                add2 = v * (j + v > -1 && j+v < s->height);

                curr_cell = oldCanvas[(j + add2) * (s->width) + i + add1];
                unpack(comps, &curr_cell);
                for (int k = 0; k < 4; k++)
                {
                    op(&vals[k], h1[(c1 + v) * s1 + c1 + u] * comps[k]);
                }
                }
            }
            for (int v = 0; v < 4; v++)
            {
                vals[v] *= norm1;
                comps[v] = vals[v];
            }
            s->pixels[j * (s->width) + i] = 0;
            pack(comps, &(s->pixels[j * (s->width) + i]));
        }
    }
}

void jitter_1d_filter(Canvas* s, u32 s1, FilterDirection fd)
{
    u32 oldCanvas[s->width*s->height];
    i32 luck = 0;
    i32 c1 = s1/2;
    u32 curr_cell = 0;
    u8 comps[4];

    for (int i = 0; i < s->width * s->height; i++)
    {
        oldCanvas[i] = s->pixels[i];
    }
    for (int j = 0; j < s->height; j++)
    {
        for (int i = 0; i < s->width; i++)
        {
                luck = rand_int_bound(0, s1) - c1;
                int add1 = (fd == FilterX) * luck * (i + luck > -1 && i+luck < s->width);
                int add2 = (fd == FilterY) * luck * (j + luck > -1 && j+luck < s->height);
                curr_cell = oldCanvas[(j + add2) * (s->width) + i + add1];
                unpack(comps, &curr_cell);
                s->pixels[j * (s->width) + i] = 0;
                pack(comps, &(s->pixels[j * (s->width) + i]));
        }
    }
}

void jitter_2d_filter(Canvas* s, u32 s1)
{
    u32 oldCanvas[s->width*s->height];
    i32 luckx = 0;
    i32 lucky = 0;
    i32 c1 = s1/2;
    u32 curr_cell = 0;
    u8 comps[4];

    for (int i = 0; i < s->width * s->height; i++)
    {
        oldCanvas[i] = s->pixels[i];
    }
    for (int j = 0; j < s->height; j++)
    {
        for (int i = 0; i < s->width; i++)
        {
                luckx = rand_int_bound(0, s1 * s1);
                lucky = luckx / s1 - c1;
                luckx %= s1;
                luckx -= c1;
                int add1 =  luckx * (i + luckx > -1 && i+luckx < s->width);
                int add2 =  lucky * (j + lucky > -1 && j+lucky < s->height);
                //printf("add1: %d, add2: %d\n", add1, add2);
                curr_cell = oldCanvas[(j + add2) * (s->width) + i + add1];
                unpack(comps, &curr_cell);
                s->pixels[j * (s->width) + i] = 0;
                pack(comps, &(s->pixels[j * (s->width) + i]));
        }
    }
}

void signed_general_linear_1d_filter(i64* s, u32 w, u32 h, i32* h1, u32 s1, 
        FilterDirection fd)
{
    i64* oldCanvas = malloc(w * h * sizeof(i64));
    i32 c1 = s1/2;
    i64 curr_cell = 0;
    f32 norm1 = 0;
    i16 vals[4] = {0, 0, 0, 0};
    i16 comps[4] = {0, 0, 0, 0};

    for (int i = 0; i < s1; i++)
    {
        norm1 += abs(h1[i]);
    }
    norm1 = 1.0f/norm1;

    for (int i = 0; i < w * h; i++)
    {
        oldCanvas[i] = s[i];
    }
    for (int j = 0; j < h; j++)
    {
        for (int i = 0; i < w; i++)
        {
            for (int v = 0; v < 3; v++)
            {
                vals[v] = 0;
            }
            for (int u = -c1; u < c1 + (s1 % 2 == 1); u++)
            {
                int add1 = (fd == FilterX) * u * (i + u > -1 && i+u < w);
                int add2 = (fd == FilterY) * u * (j + u > -1 && j+u < h);
                curr_cell = oldCanvas[(j + add2) * (w) + i + add1];

                signed_unpack(comps, &curr_cell);
                for (int v = 0; v < 3; v++)
                {
                    vals[v] += (h1[c1 +u] * comps[v]);
                }
                vals[3] = comps[3];
            }
            s[j * (w) + i] = 0;
            for (int v = 0; v < 3; v++)
            {
                vals[v] *= norm1;
            }
            signed_pack(vals, &s[j * w + i]);
        }
    }
    free(oldCanvas);
}
void general_linear_1d_filter(Canvas* s, i32* h1, u32 s1, FilterDirection fd)
{
    u32* oldCanvas = malloc(s->width * s->height * sizeof(u32));
    i32 c1 = s1/2;
    u32 curr_cell = 0;
    f32 norm1 = 0;
    u8 comps[4];
    i16 vals[4] = {0, 0, 0, 0};

    for (int i = 0; i < s1; i++)
    {
        norm1 += abs(h1[i]);
    }
    norm1 = 1.0f/norm1;

    for (int i = 0; i < s->width * s->height; i++)
    {
        oldCanvas[i] = s->pixels[i];
    }
    for (int j = 0; j < s->height; j++)
    {
        for (int i = 0; i < s->width; i++)
        {
            for (int v = 0; v < 3; v++)
            {
                vals[v] = 0;
            }
            for (int u = -c1; u < c1 + (s1 % 2 == 1); u++)
            {
                int add1 = (fd == FilterX) * u * (i + u > -1 && i+u < s->width);
                int add2 = (fd == FilterY) * u * (j + u > -1 && j+u < s->height);
                curr_cell = oldCanvas[(j + add2) * (s->width) + i + add1];

                unpack(comps, &curr_cell);
                for (int v = 0; v < 3; v++)
                {
                    vals[v] += (h1[c1 +u] * (i16)comps[v]);
                }
            }
            for (int v = 0; v < 3; v++)
            {
                vals[v] *= norm1;
                comps[v] = vals[v] * (vals[v] > 0);
            }
            s->pixels[j * (s->width) + i] = 0;
            pack(comps, &(s->pixels[j * (s->width) + i]));
        }
    }
    free(oldCanvas);
}

void general_linear_separated_filter(Canvas* s, i32* h1, u32 s1, i32* h2, u32 s2)
{
   general_linear_1d_filter(s, h1, s1, FilterX);
   general_linear_1d_filter(s, h2, s2, FilterY);
}

void box_filter(Canvas *s, u32 dim)
{
    i32* box = malloc(dim * sizeof(u32));
    for (int i = 0; i < dim; i++)
    {
        box[i] = 1;
    }
    general_linear_separated_filter(s, box, dim, box, dim);
    free(box);
}
// f = 0.219482


u32 factorial(u32 v)
{
    u32 curr = 1;
    for (u32 i = 2; i < v; i++)
    {
        curr *= i;
    }
    return curr * v * (v>0) + !(v>0);
}
void binomial_filter(Canvas* s, u32 rad)
{
    i32* bin = malloc((2 * rad + 1) * sizeof(u32));
    for (int i = 0; i < 2*rad + 1; i++)
    {
          bin[i] = factorial(2 * rad) / (factorial(i) * factorial(2*rad - i));
          //printf("%d\n", bin[i]);
    }
    general_linear_separated_filter(s, bin, 2*rad + 1, bin, 2*rad + 1);
    //free(gauss);
}

void min_filter(Canvas *s, u32 dim)
{
    i32* h = malloc(dim * dim * sizeof(u32));
    for (int k = 0; k < dim*dim; k++)
    {
        h[k] = 1;
    }
    general_2d_generic_operator_filter(s, h, dim, UINT32_MAX, 1, inplace_min);
    free(h);
}
void max_filter(Canvas *s, u32 dim)
{
    i32* h = malloc(dim * dim * sizeof(u32));
    for (int k = 0; k < dim*dim; k++)
    {
        h[k] = 1;
    }
    general_2d_generic_operator_filter(s, h, dim, 0, 1, inplace_max);
    free(h);
}

void jitter_filter(Canvas* s, u32 dim)
{
    jitter_1d_filter(s, dim, FilterX);
    jitter_1d_filter(s, dim, FilterY);
}


void grey_scale_ordered_dithering(Canvas* s, u8* d, u32 dim)
{
    u32 oldCanvas[s->width*s->height];
    u32 curr_cell = 0;
    u8 comps[4];
    f32 intensity;

    for (int i = 0; i < s->width * s->height; i++)
    {
        oldCanvas[i] = s->pixels[i];
    }
    for (int j = 0; j < s->height - dim; j += dim)
    {
        for (int i = 0; i < s->width - dim; i += dim)
        {
            for (int v = 0; v < dim; v++)
            {
                for (int u = 0; u < dim; u++)
                {
                    curr_cell = oldCanvas[(j + v) * (s->width) + i + u];
                    unpack(comps, &curr_cell);
                    intensity = 
                        0.299f * comps[0] + 0.587 * comps[1] + (0.144 * comps[2]);
                        for (int k = 0; k < 3; k++)
                        {
                                comps[k] = 0xff * !(intensity < d[v * dim + u]);
                        }
                    comps[3] = 0xff;
                    s->pixels[(j + v) * (s->width) + (i + u)] = 0;
                    pack(comps, &(s->pixels[(j + v) * (s->width) + (i + u)]));
                }
            }
        }
    }
}
void ordered_dithering(Canvas* s, u8* d, u32 dim)
{
    u32 oldCanvas[s->width*s->height];
    u32 curr_cell = 0;
    u8 comps[4];

    for (int i = 0; i < s->width * s->height; i++)
    {
        oldCanvas[i] = s->pixels[i];
    }
    for (int j = 0; j < s->height - dim; j += dim)
    {
        for (int i = 0; i < s->width - dim; i += dim)
        {
            for (int v = 0; v < dim; v++)
            {
                for (int u = 0; u < dim; u++)
                {
                    curr_cell = oldCanvas[(j + v) * (s->width) + i + u];
                    unpack(comps, &curr_cell);
                    for (int k = 0; k < 3; k++)
                    {
                            comps[k] = 0xff * !(comps[k] < d[v * dim + u]);
                    }
                    comps[3] = 0xff;
                    s->pixels[(j + v) * (s->width) + (i + u)] = 0;
                    pack(comps, &(s->pixels[(j + v) * (s->width) + (i + u)]));
                }
            }
        }
    }
}
void default_ordered_dithering(Canvas* s)
{
    u32 dim = 5;
    u8* d = malloc(dim * dim * sizeof(u8));
    for (int i = 0; i < dim * dim; i++)
    {
        d[i] = i * 0xff / (dim * dim - 1);
    }
    ordered_dithering(s, d, dim);
    free(d);
}

void default_grey_scale_ordered_dithering(Canvas* s)
{
    u32 dim = 5;
    u8* d = malloc(dim * dim * sizeof(u8));
    for (int i = 0; i < dim * dim; i++)
    {
        d[i] = i * 0xff / (dim * dim - 1);
    }
    grey_scale_ordered_dithering(s, d, dim);
    free(d);
}

void grey_scale_floyd_stienberg(Canvas* s, u8* d,u32 length)
{
    grey_scale(s);
    floyd_stienberg(s, d, length);
}
void floyd_stienberg(Canvas* s, u8* d, u32 length)
{
    u32 curr_cell = 0;
    u8 comps[4];
    u8 neighbour_comps[4];
    f32 err_factors[4] = {7.0f/16, 3.f/16, 5.f/16, 1.f/16};
    i8 offsets[8] = {1, 0, -1, 1, 0, 1, 1, 1};
    i16 min_diff[3] = {0xff, 0xff, 0xff};
    u8 approx[3];
    i16 curr_diff;

    for (int j = 0; j < s->height - 1; j ++)
    {
        for (int i = 1; i < s->width - 1; i++)
        {
                    curr_cell = j * (s->width) + i;

                    unpack(comps, &s->pixels[curr_cell]);
                    for (int k = 0; k < 3; k++)
                    {
                        min_diff[k] = 0xff;
                        approx[k] = 0xff;
                        for (int l = 0; l < length; l++)
                        {
                            curr_diff = (i16)comps[k] - (i16)d[l];
                            approx[k] = d[l] 
                                * !(abs(curr_diff) > abs(min_diff[k])) + 
                                approx[k] 
                                * (abs(curr_diff) > abs(min_diff[k]));

                            min_diff[k] = curr_diff * 
                                !(abs(curr_diff) > abs(min_diff[k]))
                                + min_diff[k] * 
                                (abs(curr_diff) > abs(min_diff[k]));
                        }
                            comps[k] = approx[k];
                    }
                    comps[3] = 0xff;
                    s->pixels[curr_cell] = 0;
                    pack(comps, &(s->pixels[curr_cell]));
                    for (int l = 0; l < 8; l+=2)
                    {
                        unpack(neighbour_comps, 
                                &s->pixels[(j + offsets[l + 1])
                                * s->width + i + offsets[l]]);
                        for (int y = 0; y < 3; y++)
                        {
                            /*i16 test = (i16)neighbour_comps[y]
                                + err_factors[l/2] * min_diff[y];
                            if (test > 0)
                            {
                            printf("TEST: %d\n", test);
                            }
                            */
                            neighbour_comps[y] = fmax(0, fmin(0xff, 
                                    (i16)neighbour_comps[y] + 
                                    err_factors[l/2] * min_diff[y]));
                        }
                        neighbour_comps[3] = 0xff;
                        s->pixels[(j + offsets[l + 1])
                            * s->width + i + offsets[l]] = 0;
                        pack(neighbour_comps, 
                                &s->pixels[(j + offsets[l + 1])
                                * s->width + i + offsets[l]]);
                    }
        }
    }
}
void line_floyd_steinberg(Canvas* s, u8 * d, u32 length, 
        i32 x_line_offset, i32 y_line_offset,
        u32 threshold)
{
    u32 curr_cell = 0;
    u8 comps[4];
    u8 neighbour_comps[4];
    f32 err_factors[4] = {7.0f/16, 3.f/16, 5.f/16, 1.f/16};
    i8 offsets[8] = {1, 0, -1, 1, 0, 1, 1, 1};
    i16 min_diff[3] = {0xff, 0xff, 0xff};
    u8 approx[3];
    i16 curr_diff;

    for (int j = 0; j < s->height - 1; j ++)
    {
        for (int i = 1; i < s->width - 1; i++)
        {
                    curr_cell = j * (s->width) + i;

                    unpack(comps, &s->pixels[curr_cell]);
                    for (int k = 0; k < 3; k++)
                    {
                        min_diff[k] = 0xff;
                        approx[k] = 0xff;
                        for (int l = 0; l < length; l++)
                        {
                            curr_diff = (i16)comps[k] - (i16)d[l];
                            approx[k] = d[l] 
                                * !(abs(curr_diff) > abs(min_diff[k])) + 
                                approx[k] 
                                * (abs(curr_diff) > abs(min_diff[k]));

                            min_diff[k] = curr_diff * 
                                !(abs(curr_diff) > abs(min_diff[k]))
                                + min_diff[k] * 
                                (abs(curr_diff) > abs(min_diff[k]));
                        }
                            comps[k] = approx[k];
                    }
                    comps[3] = 0xff;
                    int draw_line = (approx[0] >= threshold);
                    if (draw_line)
                    {
                        line(s, i - x_line_offset / 2, j - y_line_offset / 2, 
                                i + x_line_offset / 2 , j + y_line_offset / 2, 
                                0xff000000);
                    for (int k = 0; k < 3; k++)
                    {
                        min_diff[k] += 
                            (fmax(abs(y_line_offset), abs(x_line_offset)) - 1)
                            * (min_diff[k] >= 0) + 
                            - (fmax(abs(y_line_offset), abs(x_line_offset))) * (min_diff[k] < 0);
                    }
                    }
                    else 
                    {
                        for (int k = 0; k < 3; k++)
                        {
                        approx[k] = 0;
                        min_diff[k] = comps[k];
                        }
                    }

                    for (int l = 0; l < 8; l+=2)
                    {
                        unpack(neighbour_comps, 
                                &s->pixels[(j + offsets[l + 1])
                                * s->width + i + offsets[l]]);
                        for (int y = 0; y < 3; y++)
                        {
                            neighbour_comps[y] = fmax(0, fmin(0xff, 
                                    (i16)neighbour_comps[y] + 
                                    err_factors[l/2] * min_diff[y]));
                        }
                        neighbour_comps[3] = 0xff;
                        s->pixels[(j + offsets[l + 1])
                            * s->width + i + offsets[l]] = 0;
                        pack(neighbour_comps, 
                                &s->pixels[(j + offsets[l + 1])
                                * s->width + i + offsets[l]]);
                    }
        }
    }
}
void default_line_floyd_steinberg(Canvas *s, u32 threshold)
{
    grey_scale(s);
    u32 dim = 5;
    i32 x_off = -10;
    i32 y_off = 5;
    u8* d = malloc(dim * sizeof(u8));
    for (int i = 0; i < dim; i++)
    {
        d[i] = i * 0xff / (dim - 1);
    }
    line_floyd_steinberg(s, d, dim, x_off, y_off, threshold);
    free(d);
}
void default_grey_scale_floyd_steinberg(Canvas* s)
{
    grey_scale(s);
    default_floyd_steinberg(s);
}
void default_floyd_steinberg(Canvas* s)
{
    u32 dim = 5;
    u8* d = malloc(dim * sizeof(u8));
    for (int i = 0; i < dim; i++)
    {
        d[i] = i * 0xff / (dim - 1);
    }
    floyd_stienberg(s, d, dim);
    free(d);
}
void median_filter(Canvas* s, u32 dim)
{
}

void sobel_filter(Canvas* s)
{
    i64* pixh = malloc(s->width*s->height * sizeof(i64));
    i64* pixv = malloc(s->width*s->height * sizeof(i64));

    i16 comps1[4] = {0, 0, 0, 0};
    i16 comps2[4] = {0, 0, 0, 0};
    for (int i = 0; i < s->width * s->height; i++)
    {
        unsigned_to_signed(&pixh[i], &s->pixels[i]);
        unsigned_to_signed(&pixv[i], &s->pixels[i]);
    }

    i32 grad[3] = {-1, 0, 1};
    i32 blur[3] = {1, 2, 1};

    signed_general_linear_1d_filter(pixh,s->width, s->height, blur, 3, FilterY);
    signed_general_linear_1d_filter(pixh,s->width, s->height, grad, 3, FilterX);
    signed_general_linear_1d_filter(pixv,s->width, s->height, blur, 3, FilterX);
    signed_general_linear_1d_filter(pixv,s->width, s->height, grad, 3, FilterY);


    for (int j = 0; j < s->width * s->height; j++)
    {
        signed_unpack(comps1, &pixh[j]);
        signed_unpack(comps2, &pixv[j]);
        (void)comps2;
        for (int i = 0; i < 3; i++)
        {
            comps1[i] = 
                sqrt(((f32)comps1[i] * comps1[i] 
                            + (f32)comps2[i] + comps2[i]));
        }
        pixh[j] = 0;
        signed_pack(comps1, &pixh[j]);
        signed_to_unsigned(&pixh[j], &s->pixels[j]);
    }
}

void canny_filter(Canvas* s, u8 t_low, u8 t_high)
{
    u8 temp = t_low;
    t_low = t_low * (t_low < t_high) + t_high * !(t_low < t_high);
    t_high = t_high * (temp < t_high) + temp * !(temp < t_high);
    i64* x_grad = malloc(s->width*s->height * sizeof(i64));
    i64* y_grad = malloc(s->width*s->height * sizeof(i64));
    u32* mag = malloc(s->width*s->height * sizeof(u32));
    u32* maximas = malloc(s->width*s->height * sizeof(u32));
    (void)maximas;
    i16 x_comps[4] = {0, 0, 0, 0};
    i16 y_comps[4] = {0, 0, 0, 0};
    u8 u_comps[4] = {0, 0, 0, 0};
    u8 orientation = 0;

    binomial_filter(s, 3);

    for (int i = 0; i < s->width * s->height; i++)
    {
        unsigned_to_signed(&x_grad[i], &s->pixels[i]);
        unsigned_to_signed(&y_grad[i], &s->pixels[i]);
        mag[i] = 0;
        maximas[i] = 0;
        s->pixels[i] = 0;
    }

    i32 grad_kernel[3] = {-1, 0, 1};
    signed_general_linear_1d_filter(x_grad, s->width, s->height, grad_kernel, 3, FilterX);
    signed_general_linear_1d_filter(y_grad, s->width, s->height, grad_kernel, 3, FilterY);

    for (int j = 0; j < s->height; j++)
    {
        for (int i = 0; i < s->width; i++)
        {
            signed_unpack(x_comps, &x_grad[j * s->width + i]);
            signed_unpack(y_comps, &y_grad[j * s->width + i]);
            u_comps[3] = x_comps[3];
        for (int v = 0; v < 3; v++)
        {
            u_comps[v] = 
                sqrt((f32)((f32)x_comps[v] * x_comps[v] 
                            + (f32)y_comps[v] + y_comps[v]));
        }
        pack(u_comps, &mag[j * s->width + i]);
        }
    }
    for (u32 j = 1; j < s->height - 1; j++)
    {
        for (u32 i = 1; i < s->width - 1; i++)
        {
           orientation = canny_get_orientation(x_grad[j * s->width + i], 
                   y_grad[j * s->width + i]);
           if (canny_is_local_max(mag, s->width, i, j, orientation, t_low))
           {
            //    maximas[j * s->width + i] = mag[j * s->width + i];
                s->pixels[j * s->width + i] = mag[j * s->width + i];
           }
        }
    }

    for (u32 j = 1; j < s->height - 1; j++)
    {
        for (u32 i = 1; i < s->width - 1; i++)
        {
            if (compare_with_comps(maximas[j * s->width + i], t_high) != -1 
                    &&  s->pixels[j * s->width + i] == 0)
            {
                printf("HERE\n");
                canny_trace_contour(maximas, mag, 
                        s->pixels, s->width, s->height, i, j, t_low); 
            }
        }
        }
    free(x_grad);
    free(y_grad);
    free(maximas);
    free(mag);
}

u8 canny_get_orientation(i64 dx, i64 dy)
{
    f64 tdx = (f64)dx;
    f64 tdy = (f64)dy;
    tdx = COS_PI_OVER_8 * tdx + -SIN_PI_OVER_8 * tdy;
    tdy = COS_PI_OVER_8 * tdy + SIN_PI_OVER_8 * tdx;
    tdx = tdx * (tdy >=0) - tdx * (tdy < 0);
    tdy = tdy * (tdy >=0) - tdy * (tdy < 0);

    return 
        (dx >= 0 && tdx < tdy) 
        + 2 * (dx < 0 && -dx < dy) + 
        3 * (dx < 0 && -dx >= dy);
}

u8 canny_is_local_max(u32* mag, u32 w, u32 x, u32 y, u32 orientation, u8 t_lo)
{
    if (compare_with_comps(mag[y * w + x], t_lo) == -1) return 0;
    u32 ml = 0;
    u32 mh = 0;
    switch (orientation)
    {
        case 0:
            {
                ml = mag[y * w + (x-1)];
                mh = mag[y * w + (x+1)];
            }
            break;
        case 1:
            {
                ml = mag[(y-1) * w + (x-1)];
                mh = mag[(y+1) * w + (x+1)];
            }
            break;
        case 2:
            {
                ml = mag[(y-1) * w + x];
                mh = mag[(y+1) * w + x];
            }
            break;
        case 3:
            {
                ml = mag[(y+1) * w + x - 1];
                mh = mag[(y-1) * w + x + 1];
            }
            break;
    }
    return (ml <= mag[y*w + x]) && (mh <= mag[y*w + x]);
}

void canny_trace_contour(u32* maximas, u32* mag, u32* contour,
        u32 w, u32 h,  u32 x, u32 y, u8 t_low)
{
    contour[y * w + x] = mag[y * w + x];
    u32 newminx = (x-1) * (((i64)x - 1) >= 0);
    u32 newmaxx = (x+1) * ((x + 1) <= (w-1)) + (w-1) * !((x+1) <= (w-1));
    u32 newminy = (y-1) * (((i64)y - 1) >= 0);
    u32 newmaxy = (y+1) * ((y + 1) <= (h-1)) + (h-1) * !((y+1) <= (h-1));

    for (u32 xx = newminx; xx < newmaxx + 1; xx++)
    {
        for (u32 yy = newminy; yy < newmaxy + 1; yy++)
        {
            if (compare_with_comps(maximas[yy * w + xx], t_low) != -1 && 
                    contour[yy * w + x] == 0)
            {
                canny_trace_contour(maximas, mag, contour, w, h, xx, yy, t_low);
            }
        }
    }
    return;
}
