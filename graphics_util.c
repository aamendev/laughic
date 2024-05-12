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
