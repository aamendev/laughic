#ifndef PROCESSING_H
#define PROCESSING_H
#include "graphics_util.h"

// Modifiers
void mult_contrast(Canvas* c, float val);
void inc_channel(Canvas*, Channel channel, i16 val);
void inc_brightness(Canvas* c, i16 val);
void invert(Canvas* c);
void filter(Canvas* c, Channel channel);
void filter_threshold(Canvas* c, Channel channel, u8 t);
void clamp_channel(Canvas*, Channel channel, u8 t0, u8 t1);
void clamp_all(Canvas*, u8 t0, u8 t1);
void grey_scale(Canvas*);

// TODO: Histogram Methods
//void generate_histograms(Canvas* c);

// Filters:

typedef enum FilterDirection
{
    FilterX,
    FilterY
} FilterDirection;

void signed_general_linear_1d_filter(i64* s, u32 w, u32 h, i32* h1, u32 s1, FilterDirection);
void general_linear_1d_filter(Canvas* s, i32* h1, u32 s1, FilterDirection);
void general_linear_separated_filter(Canvas* s, i32* h1, u32 s1, i32* h2, u32 s2);
void general_2d_generic_operator_filter(Canvas* s, i32* h1, u32 s1, 
        u32 val_init, i32 normalize_factor, void (*op)(u32* a, u32 b));
void jitter_1d_filter(Canvas* s, u32 s1, FilterDirection fd);
void jitter_2d_filter(Canvas* s, u32 s1);
void box_filter(Canvas* s, u32 dim);
void binomial_filter(Canvas* s, u32 rad);
void gaussian_filter(Canvas* s, u32 rad);
void min_filter(Canvas* s, u32 dim);
void max_filter(Canvas* s, u32 dim);
void jitter_filter(Canvas* s, u32 dim);
void median_filter(Canvas* s, u32 dim);
void sobel_filter(Canvas* s);
void canny_filter(Canvas* s, u8 t_low, u8 t_high);


// Helpers
//
u8 canny_get_orientation(i64 dx, i64 dy);
u8 canny_is_local_max(u32* mag, u32 w, u32 x, u32 y, u32 orientation, u8 t_lo);
void canny_trace_contour(u32* maximas, u32* mag,
        u32* contour, u32 w, u32 h, u32 x, u32 y, u8 t_low);
#endif
