#include "./logic_util.h"
#include "./graphics_util.h"
#ifndef SHAPESH
#define SHAPESH

typedef struct Line
{
    int x0;
    int y0;
    int x1;
    int y1;
    u32 colour;
} Line;

typedef struct Circle
{
    int r;
    int cx;
    int cy;
    u32 colour;
} Circle;

typedef struct Rectangle
{
    int x;
    int y;
    int w;
    int h;
    u32 colour;
} Rectangle;

typedef struct Ellipse
{
   int x;
   int y;
   int r0;
   int r1;
   u32 colour;
} Ellipse;

typedef struct Triangle
{
    int x0;
    int y0;
    int x1;
    int y1;
    int x2;
    int y2;
    u32 colour;
} Triangle;


typedef enum Shape
{
    LINE,
    RECTANGLE,
    CIRCLE,
    ELLIPSE,
    TRIANGLE,
    TOTAL
} Shape;
void point(Canvas* canvas, int x, int y, u32 colour);
void line_old(Canvas* canvas, int x0, int y0, int x1, int y1, u32 colour);
void line(Canvas* canvas, int x0, int y0, int x1, int y1, u32 colour);

void fill(Canvas* canvas, u32 colour);

void rect(Canvas* canvas, int x, int y, int w, int h, u32 colour);
void fill_rect(Canvas* canvas, int x, int y, int w, int h, u32 colour);
void fill_rect_texture(Canvas* canvas, int x, int y, int w, int h, Texture* tex);

void circle(Canvas* canvas, int x, int y, int r, u32 colour);
void fill_circle(Canvas* canvas, int x, int y, int r, u32 colour);
void fill_circle_texture(Canvas* canvas, int x, int y, int r, Texture* tex);

void ellipse(Canvas* canvas, int x, int y, int r0, int r1, u32 colour);
void fill_ellipse(Canvas* canvas, int x, int y, int r0, int r1, u32 colour);
void fill_ellipse_texture(Canvas* canvas, int x, int y, int r0, int r1, Texture* tex);

void triangle(Canvas* canvas, int x0, int y0, int x1, int y1, int x2, int y2, u32 colour);
void fill_triangle(Canvas* canvas, int x1, int y1, int x2, int y2, int x3, int y3, u32 color);
void fill_triangle_texture(Canvas* canvas, int x1, int y1, int x2, int y2, int x3, int y3, Texture* tex, int u0, int v0, int u1, int v1, int u2, int v2);
void fill_triangle_bary(Canvas* canvas, int x1, int y1, int x2, int y2, int x3, int y3, u32 c1, u32 c2, u32 c3);

//void fill(Canvas* canvas, Shape type, void* shape);

#endif

