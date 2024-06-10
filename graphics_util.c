#include "types.h"
#include <stdio.h>
#include "./graphics_util.h"

void unpack(u8* comp, u32* c)
{
        comp[0] = *c&0XFF;
        comp[1] = (*c>>8) & 0XFF;
        comp[2] = (*c >> (8 * 2)) & 0xFF;
        comp[3] = (*c >> (8 * 3)) & 0xFF;
}

void mix_component(u8* c1, u8 c2, u8 a)
{
    *c1 =  *c1 + (c2 - *c1) * a / 255;
}

void mix_colour(u32* c1, u32 c2)
{
    u8 comp1[4];
    unpack(comp1, c1);
    *c1 = 0;
    u8 comp2[4];
    unpack(comp2, &c2);
    for (int i = 0; i < 4; i++)
    {
        mix_component(&comp1[i], comp2[i], comp2[3]);
        *c1 |= comp1[i]<<(8 * i);
    }
}
u32 complement(u32 colour)
{
    u32 c = 0;
    u8 r = 0xFF - ((colour)&0xFF);
    u8 g = 0xFF - ((colour>>8)&0xFF);
    u8 b = 0xFF - ((colour>>(8 * 2))&0xFF);
    u8 a = (colour>>(8 * 3))&0xFF;
    c |= (r | g<<8 | b<<(8 * 2) | a<<(8 * 3));
    return c;
}

void resize(Texture* src, u32 wn, u32 hn, u32** dst)
{
    *dst = (u32*)malloc(wn * hn * sizeof(u32));
    for (u32 j = 0; j < hn; j++)
    {
        for (u32 i = 0; i < wn; i++)
        {
            dst[0][j * wn + i] = src->data[j * src->height/hn * src->width + i * src->width/ wn];
        }
    }
}

void mandelbrot(Canvas* canvas, u16 iterations, u32 base_colour)
{
    //z_n+1 = zn^2 + c
    for (int i = 0; i < canvas->width * canvas->height; i++)
    {
        float x0 = 2.47 * (i % (canvas->width)) / canvas->width - 2;
        float y0 = 2.24 * (i / (canvas->width)) / canvas->height - 1.12;
        float currentX = 0;
        float currentY = 0;
        const float bound  = 4;
        int iter = 0;
        (void)x0;
        (void)y0;
        while (iter < iterations && currentX * currentX + currentY * currentY < bound)
        {
            float tempx = currentX * currentX - currentY * currentY;
            currentY = 2 * currentX * currentY + y0;
            currentX = tempx + x0;
            iter++;
        }
        mix_colour(&canvas->pixels[i] , (1 - (float)iter / iterations) *  base_colour);
    }
}

void julia(Canvas* canvas, u16 iterations, u32 base_colour, float zx, float zy)
{
    //z_n+1 = zn^2 + c
    const float r = 2; 
    for (int i = 0; i < canvas->width * canvas->height; i++)
    {

        float x0 = 2 * r * (i % (canvas->width)) / canvas->width - r;
        float y0 = 2 * r * (i / (canvas->width)) / canvas->height - r; 
        int iter = 0;
        while (iter < iterations && x0 * x0 + y0 * y0 < r * r)
        {
            float tempx = x0 * x0 - y0 * y0;
            y0 = 2 * x0 * y0 + zy;
            x0 = tempx + zx;
            iter++;
        }
        mix_colour(&canvas->pixels[i] , (1 - (float)iter / iterations) *  base_colour);
    }
}
