#include "types.h"
#include <math.h>
#include <stdio.h>
#include "./graphics_util.h"


void signed_unpack(i16* comp, i64* c)
{
    for (int i = 0; i < 4; i++)
    {
        comp[i] = (i16)((*c >> (16 * i)) & 0xFFFF);
    }
}

void signed_pack(i16* comp, i64* c)
{
    for (int i = 0; i < 4; i++)
    {
        *c |= ((i64)comp[i]<<(16 * i));
    }
}

void signed_to_unsigned(i64* sgn, u32* un)
{
   i16 sgn_comp[4] = {0,0,0,0}; 
   u8 un_comp[4] = {0, 0, 0 ,0};
   signed_unpack(sgn_comp, sgn);
   for (int i = 0; i < 4; i++)
   {
        un_comp[i] = (u8)sgn_comp[i] * (sgn_comp[i] > -1); 
   }
   *un = 0;
   pack(un_comp, un);
}

void unsigned_to_signed(i64* sgn, u32* un)
{
   i16 sgn_comp[4] = {0,0,0,0}; 
   u8 un_comp[4] = {0, 0, 0 ,0};
   unpack(un_comp, un);
   for (int i = 0; i < 4; i++)
   {
        sgn_comp[i] = un_comp[i]; 
        //printf("Old: %x, New: %x\n",un_comp[i], sgn_comp[i]);
   }
   *sgn = 0;
   signed_pack(sgn_comp, sgn);
   //printf("Old: %x, New: %lx\n",*un, *sgn);
}
void unpack(u8* comp, u32* c)
{
        comp[0] = *c&0XFF;
        comp[1] = (*c>>8) & 0XFF;
        comp[2] = (*c >> (8 * 2)) & 0xFF;
        comp[3] = (*c >> (8 * 3)) & 0xFF;
}

void pack(u8* comp, u32* c)
{
    for (int i = 0; i < 4; i++)
    {
        *c |= comp[i]<<(8 * i);
    }
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

u32 weighted_sum(u32* cols, float* means, u32 count)
{
    u32 current = 0;
    u8 comps[4] = {0, 0, 0, 0};
    u8 out[4] = {0, 0 , 0 , 0};
    float sum = 0;
    for (u32 i = 0; i < count; i++)
    {
        unpack(comps, &cols[i]);
        for (int j = 0; j < 4; j++)
            out[j] += (comps[j] * means[i]);
        sum += means[i];
    }
    pack(out, &current);
    return current;
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

void julia(Canvas* canvas, u16 iterations, float zx, float zy, u32 base_colour)
{
    //z_n+1 = zn^2 + c
    float shift = 1.5 * 3.1415;
    const float r = 1.7; 
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
    u8 comp[4] = {0,0,0,0};
    float freq[3] = {0.03, 0.018, 0.019};
    float phase[3] = {0.0, 0.0, 0.0};
    float continousFactor = iter + 1 - 1 /(zx * zx + zy * zy);
    base_colour = 0xff000000;
    for (int i = 0; i < 3; i++)
    {
        comp[i] = (0.5 * sin(freq[i] * continousFactor + phase[i] + shift) + 0.5) * 255;
        base_colour |= (comp[i]<<8*i);
    }
        mix_colour(&canvas->pixels[i], base_colour);
    }
}

void newton(Canvas* canvas, u16 iterations, u32 base_colour)
{
    //z = z^3 - 1
    float roots[6] = {1, 0, -0.5f, sqrt(3) / 2, -0.5, - sqrt(3) / 2};
    for (int i = 0; i < canvas->width * canvas-> height; i++)
    {
        float x0 = 4.0f * (i % (canvas->width)) / canvas->width - 2;
        float y0 = 4.0f * ((i / canvas->width)) / canvas->width - 2;
        int iter = 0;
        while (iter < iterations)
        {
            //z^2 = x^2 - y^2 +2xyi
            float derix = 3 * (x0 * x0 - y0 * y0);
            float deriy = 3 * 2 * x0 * y0;
            // z -= z ^ 3 - 1 / deri
            //z ^ 3 =(x + yi) ^ 3 = x^3 + 3x^2yi  - 3xy^2 - y^3i 
            float z3x = x0 * x0 * x0 - 3 * x0 * y0 * y0 - 1;
            float z3y = 3 * x0 * x0 * y0 - y0 * y0 * y0;

            x0 -= (z3x * derix + deriy * z3y) / (derix * derix + deriy * deriy);
            y0 -= (z3y * derix - deriy * z3x) / (derix * derix + deriy * deriy);

            float tol = 0.000001f;
            for (int i = 0; i < 3; i++)
            {
                float difx = x0 - roots[2 * i];
                float dify = y0 - roots[2 * i + 1];
                difx = difx * ((difx > 0) - !(difx > 0));
                dify = dify * ((dify > 0) - !(dify > 0));
                if (difx < tol && dify < tol)
                {
                    goto process;
                }
            }
            iter++;
        }
process:
        mix_colour(&canvas->pixels[i] , (1 - (float)iter / iterations) *  base_colour);
    }
}
void newton2(Canvas *canvas, u16 iterations, void (*func)(float, float, float *, float *), void (*deri)(float, float, float *, float *), float* roots, u16 nRoots)
 {
    u32 base_colour = 0xff000000;
    float derix = 0.0f;
    float deriy = 0.0f;
    float zx = 0.0f;
    float zy = 0.0f;

    for (int i = 0; i < canvas->width * canvas-> height; i++)
    {
        float x0 = 4.0f * (i % (canvas->width)) / canvas->width - 2;
        float y0 = 4.0f * ((i / canvas->width)) / canvas->width - 2;
        int iter = 0;
        while (iter < iterations)
        {
            func(x0, y0, &zx, &zy);
            deri(x0, y0, &derix, &deriy);

            x0 -= (zx * derix + deriy * zy) / (derix * derix + deriy * deriy);
            y0 -= (zy * derix - deriy * zx) / (derix * derix + deriy * deriy);

            float tol = 0.000001f;
            for (int i = 0; i < nRoots; i++)
            {
                float difx = x0 - roots[2 * i];
                float dify = y0 - roots[2 * i + 1];
                difx = difx * ((difx > 0) - !(difx > 0));
                dify = dify * ((dify > 0) - !(dify > 0));
                if (difx < tol && dify < tol)
                {
                    goto process;
                }
            }
            iter++;
        }
process:
        mix_colour(&canvas->pixels[i] , (1 - (float)iter / iterations) *  base_colour);
    }
}

i8 compare_with_comps(u32 c, u8 t)
{
    u8 comp[4] = {0,0,0,0};
    unpack(comp, &c);
    u8 notEqual = 0;
    for (int i = 0; i < 3; i++)
    {
        if (comp[i] < t) return -1;
        else if (comp[i] > t)
        {
            notEqual = 1;
        }
    }
    return notEqual;
}
