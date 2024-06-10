#include <stdlib.h>
#include "types.h"
#ifndef GRAPHICSUTIL
#define GRAPHICSUTIL

typedef enum
{
    PNG,
    JPG,
    PPM,
    FORMATS
} Format;

typedef struct
{
   u32* pixels;
   size_t width;
   size_t height;
} Canvas;

typedef struct
{
    const char* path;
    u32* data;
    size_t width;
    size_t height;
} Texture;

void mix_component(u8* c1, u8 c2, u8 a);
void unpack(u8* , u32*);
void mix_colour(u32* c1, u32 c2);
u32 complement(u32 colour);
void resize(Texture* src, u32 wn, u32 hn, u32** dst);
void mandelbrot(Canvas* canvas, u16 iterations, u32 base_colour);
void julia(Canvas* canvas, u16 iterations, u32 base_colour, float zx, float zy);
#endif
