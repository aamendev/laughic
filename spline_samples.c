#include "spline_samples.h"
void bspline_face_outline(Canvas * c, SimpleBrush* sb, int x0, int y0)
{
    int bspline_offset_x = 15;
    int bspline_offset_y = 50;
    float knots[14] = {
        0,
        0,
        0,
        0,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
    };


    float l_face_xcoffs[6] = 
    {
        x0 + bspline_offset_x, 
        x0 - bspline_offset_x,
        x0 - bspline_offset_x,
        x0 + 4 * bspline_offset_x,
    };

    float l_face_ycoffs[6] = 
    {
        y0, 
        y0 + bspline_offset_y,
        y0 + 3.8f * bspline_offset_y,
        y0 + 4 * bspline_offset_y
    };
    BSpline l_face = 
    {
        .x_coeffs = l_face_xcoffs,
        .y_coeffs = l_face_ycoffs,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };
    bspline(c, &l_face, sb);
 float r_face_xcoffs[6] = 
    {
        x0 - bspline_offset_x, 
        x0 + bspline_offset_x,
        x0 + bspline_offset_x,
        x0 - 4 * bspline_offset_x,
    };

    float r_face_ycoffs[6] = 
    {
        y0, 
        y0 + bspline_offset_y,
        y0 + 3.8f * bspline_offset_y,
        y0 + 4 * bspline_offset_y
    };

    BSpline r_face = 
    {
        .x_coeffs = r_face_xcoffs,
        .y_coeffs = r_face_ycoffs,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };

    int extra_offset = 120;
    for (int i = 0; i < r_face.coeffs_count; i++)
    {
        r_face.x_coeffs[i] += extra_offset;
    }

    bspline(c, &r_face, sb);
 float l_top_face_xcoffs[6] = 
    {
        x0 + bspline_offset_x, 
        x0 + 2* bspline_offset_x,
        x0 + 3 * bspline_offset_x,
        x0 + 5 * bspline_offset_x,
    };

    bspline_offset_y /= 8;

    float l_top_face_ycoffs[6] = 
    {
        y0, 
        y0 - bspline_offset_y,
        y0 - 3.8f * bspline_offset_y,
        y0 - 4 * bspline_offset_y
    };

    BSpline l_top_face = 
    {
        .x_coeffs = l_top_face_xcoffs,
        .y_coeffs = l_top_face_ycoffs,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };
    bspline(c, &l_top_face, sb);

    float r_top_face_xcoffs[6] = 
    {
        x0 + 5 * bspline_offset_x, 
        x0 + 6 * bspline_offset_x,
        x0 + 6.5 * bspline_offset_x,
        x0 + 7 * bspline_offset_x,
    };


    float r_top_face_ycoffs[6] = 
    {
        y0 - 4 * bspline_offset_y,
        y0 - 3.8f * bspline_offset_y,
        y0 - bspline_offset_y,
        y0, 
    };

    BSpline r_top_face = 
    {
        .x_coeffs = r_top_face_xcoffs,
        .y_coeffs = r_top_face_ycoffs,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };
    bspline(c, &r_top_face, sb);
}
void bspline_l_eye(Canvas* c, SimpleBrush* sb, int x0, int y0)
{
    int bspline_offset_x = 10;
    int bspline_offset_y = 5;
    float knots[14] = {
        0,
        0,
        0,
        0,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
    };


    float l_top_x[6] = 
    {
        x0, 
        x0 + bspline_offset_x,
        x0 + 2 * bspline_offset_x,
        x0 + 3 * bspline_offset_x,
    };

    float l_top_y[6] = 
    {
        y0, 
        y0 - bspline_offset_y,
        y0 - 3.8f * bspline_offset_y,
        y0,
    };
    BSpline l_top = 
    {
        .x_coeffs = l_top_x,
        .y_coeffs = l_top_y,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };
    bspline(c, &l_top, sb);

    float l_bottom_x[6] = 
    {
        x0, 
        x0 + bspline_offset_x,
        x0 + 2 * bspline_offset_x,
        x0 + 3 * bspline_offset_x,
    };

    float l_bottom_y[6] = 
    {
        y0, 
        y0 + bspline_offset_y,
        y0 + 3.8f * bspline_offset_y,
        y0,
    };
    BSpline l_bottom = 
    {
        .x_coeffs = l_bottom_x,
        .y_coeffs = l_bottom_y,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };
    bspline(c, &l_bottom, sb);

    float internal_x[6] = 
    {
        x0 + 2 * bspline_offset_x, 
        x0 +  bspline_offset_x,
        x0 +  bspline_offset_x,
        x0 + 2 * bspline_offset_x,
    };

    float internal_y[6] = 
    {
        y0 - 2 * bspline_offset_y, 
        y0,
        y0 + bspline_offset_y,
        y0 + 2 * bspline_offset_y,
    };
    BSpline internal = 
    {
        .x_coeffs = internal_x,
        .y_coeffs = internal_y,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };
    bspline(c, &internal, sb);
}
void bspline_r_eye(Canvas* c, SimpleBrush* sb, int x0, int y0)
{
    int bspline_offset_x = 10;
    int bspline_offset_y = 5;
    float knots[14] = {
        0,
        0,
        0,
        0,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
    };


    float l_top_x[6] = 
    {
        x0, 
        x0 - bspline_offset_x,
        x0 - 2 * bspline_offset_x,
        x0 - 3 * bspline_offset_x,
    };

    float l_top_y[6] = 
    {
        y0, 
        y0 - bspline_offset_y,
        y0 - 3.8f * bspline_offset_y,
        y0,
    };
    BSpline l_top = 
    {
        .x_coeffs = l_top_x,
        .y_coeffs = l_top_y,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };
    bspline(c, &l_top, sb);

    float l_bottom_x[6] = 
    {
        x0, 
        x0 - bspline_offset_x,
        x0 - 2 * bspline_offset_x,
        x0 - 3 * bspline_offset_x,
    };

    float l_bottom_y[6] = 
    {
        y0, 
        y0 + bspline_offset_y,
        y0 + 3.8f * bspline_offset_y,
        y0,
    };
    BSpline l_bottom = 
    {
        .x_coeffs = l_bottom_x,
        .y_coeffs = l_bottom_y,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };  

    bspline(c, &l_bottom, sb);
    float internal_x[6] = 
    {
        x0 - 2 * bspline_offset_x, 
        x0 -  bspline_offset_x,
        x0 -  bspline_offset_x,
        x0 - 2 * bspline_offset_x,
    };

    float internal_y[6] = 
    {
        y0 - 2 * bspline_offset_y, 
        y0,
        y0 + bspline_offset_y,
        y0 + 2 * bspline_offset_y,
    };
    BSpline internal = 
    {
        .x_coeffs = internal_x,
        .y_coeffs = internal_y,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };
    bspline(c, &internal, sb);
}

void bspline_smile(Canvas* c, SimpleBrush* sb, int x0, int y0)
{
    int bspline_offset_x = 20;
    int bspline_offset_y = 10;
    float knots[14] = {
        0,
        0,
        0,
        0,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
    };

    float smile_x[6] = 
    {
        x0, 
        x0 + bspline_offset_x,
        x0 + 2 * bspline_offset_x,
        x0 + 3 * bspline_offset_x,
    };

    float smile_y[6] = 
    {
        y0, 
        y0 + bspline_offset_y,
        y0 +  bspline_offset_y,
        y0,
    };
    BSpline smile = 
    {
        .x_coeffs = smile_x,
        .y_coeffs = smile_y,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };
    bspline(c, &smile, sb);
}

void bspline_nose(Canvas* c, SimpleBrush* sb, int x0, int y0)
{

    int bspline_offset_x = 3;
    int bspline_offset_y = 2;
    float knots[14] = {
        0,
        0,
        0,
        0,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
    };

    float l_top_x[6] = 
    {
        x0, 
        x0 + bspline_offset_x,
        x0 + 2 * bspline_offset_x,
        x0 + 3 * bspline_offset_x,
    };

    float l_top_y[6] = 
    {
        y0, 
        y0 - bspline_offset_y,
        y0 - 2 * bspline_offset_y,
        y0 - 3 * bspline_offset_y,
    };
    BSpline l_top = 
    {
        .x_coeffs = l_top_x,
        .y_coeffs = l_top_y,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };
    bspline(c, &l_top, sb);

    int r_x_off = 15;
    float r_top_x[6] = 
    {
        x0, 
        x0 - 0.5f * bspline_offset_x,
        x0 - 1 * bspline_offset_x,
        x0 - 1.5f * bspline_offset_x,
    };

    for (int i = 0; i < 4; i++)
    {
        r_top_x[i] += r_x_off;
    }

    float r_top_y[6] = 
    {
        y0, 
        y0 - bspline_offset_y,
        y0 - 2 * bspline_offset_y,
        y0 - 3 * bspline_offset_y,
    };
    BSpline r_top = 
    {
        .x_coeffs = r_top_x,
        .y_coeffs = r_top_y,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };

    bspline(c, &r_top, sb);

    int b_offset_y = 2;

    float l_bottom_x[6] = 
    {
        x0, 
        x0 + bspline_offset_x,
        x0 + 2 * bspline_offset_x,
        x0 + 3 * bspline_offset_x,
    };

    float l_bottom_y[6] = 
    {
        y0, 
        y0 + bspline_offset_y,
        y0 + 2 * bspline_offset_y,
        y0 + 3 * bspline_offset_y,
    };

    for (int i = 0; i < 4; i++)
    {
        l_bottom_y[i] += b_offset_y;
    }
    BSpline l_bottom = 
    {
        .x_coeffs = l_bottom_x,
        .y_coeffs = l_bottom_y,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };
    bspline(c, &l_bottom, sb);

    float r_bottom_x[6] = 
    {
        x0, 
        x0 - 0.5f * bspline_offset_x,
        x0 - 1 * bspline_offset_x,
        x0 - 1.5f * bspline_offset_x,
    };

    for (int i = 0; i < 4; i++)
    {
        r_bottom_x[i] += r_x_off;
    }

    float r_bottom_y[6] = 
    {
        y0, 
        y0 + bspline_offset_y,
        y0 + 2 * bspline_offset_y,
        y0 + 3 * bspline_offset_y,
    };

    for (int i = 0; i < 4; i++)
    {
        r_bottom_y[i] += b_offset_y;
    }
    BSpline r_bottom = 
    {
        .x_coeffs = r_bottom_x,
        .y_coeffs = r_bottom_y,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };
    bspline(c, &r_bottom, sb);
}


void bspline_wiggle_face_outline(
        Canvas* c, SimpleBrush* sb, int x0, int y0, void* opts,
        void(*modify)
        (Canvas* c, 
         BSpline* bsp, float u, Line* l,  SimpleBrush* sb, void* opts))
{
    int bspline_offset_x = 15;
    int bspline_offset_y = 50;
    float knots[14] = {
        0,
        0,
        0,
        0,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
    };


    float l_face_xcoffs[6] = 
    {
        x0 + bspline_offset_x, 
        x0 - bspline_offset_x,
        x0 - bspline_offset_x,
        x0 + 4 * bspline_offset_x,
    };

    float l_face_ycoffs[6] = 
    {
        y0, 
        y0 + bspline_offset_y,
        y0 + 3.8f * bspline_offset_y,
        y0 + 4 * bspline_offset_y
    };
    BSpline l_face = 
    {
        .x_coeffs = l_face_xcoffs,
        .y_coeffs = l_face_ycoffs,
        .coeffs_count= 4,
        .seg_count = 5e1,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };
    bspline_modify(c, &l_face, sb, opts, modify);
 float r_face_xcoffs[6] = 
    {
        x0 - bspline_offset_x, 
        x0 + bspline_offset_x,
        x0 + bspline_offset_x,
        x0 - 4 * bspline_offset_x,
    };

    float r_face_ycoffs[6] = 
    {
        y0, 
        y0 + bspline_offset_y,
        y0 + 3.8f * bspline_offset_y,
        y0 + 4 * bspline_offset_y
    };

    BSpline r_face = 
    {
        .x_coeffs = r_face_xcoffs,
        .y_coeffs = r_face_ycoffs,
        .coeffs_count= 4,
        .seg_count = 5e1,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };

    int extra_offset = 120;
    for (int i = 0; i < r_face.coeffs_count; i++)
    {
        r_face.x_coeffs[i] += extra_offset;
    }

    bspline_modify(c, &r_face, sb, opts, modify);
 float l_top_face_xcoffs[6] = 
    {
        x0 + bspline_offset_x, 
        x0 + 2* bspline_offset_x,
        x0 + 3 * bspline_offset_x,
        x0 + 5 * bspline_offset_x,
    };

    bspline_offset_y /= 8;

    float l_top_face_ycoffs[6] = 
    {
        y0, 
        y0 - bspline_offset_y,
        y0 - 3.8f * bspline_offset_y,
        y0 - 4 * bspline_offset_y
    };

    BSpline l_top_face = 
    {
        .x_coeffs = l_top_face_xcoffs,
        .y_coeffs = l_top_face_ycoffs,
        .coeffs_count= 4,
        .seg_count = 5e1,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };
    bspline_modify(c, &l_top_face, sb, opts, modify);

    float r_top_face_xcoffs[6] = 
    {
        x0 + 5 * bspline_offset_x, 
        x0 + 6 * bspline_offset_x,
        x0 + 6.5 * bspline_offset_x,
        x0 + 7 * bspline_offset_x,
    };


    float r_top_face_ycoffs[6] = 
    {
        y0 - 4 * bspline_offset_y,
        y0 - 3.8f * bspline_offset_y,
        y0 - bspline_offset_y,
        y0, 
    };

    BSpline r_top_face = 
    {
        .x_coeffs = r_top_face_xcoffs,
        .y_coeffs = r_top_face_ycoffs,
        .coeffs_count= 4,
        .seg_count = 5e1,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };
    bspline_modify(c, &r_top_face, sb, opts, modify);
}

void bspline_width_l_eye(
        Canvas* c, SimpleBrush* sb, int x0, int y0, void* opts,
        void(*modify)
        (Canvas* c, 
         BSpline* bsp, float u, Line* l,  SimpleBrush* sb, void* opts))
{

    int bspline_offset_x = 10;
    int bspline_offset_y = 5;
    float knots[14] = {
        0,
        0,
        0,
        0,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
    };


    float l_top_x[6] = 
    {
        x0, 
        x0 + bspline_offset_x,
        x0 + 2 * bspline_offset_x,
        x0 + 3 * bspline_offset_x,
    };

    float l_top_y[6] = 
    {
        y0, 
        y0 - bspline_offset_y,
        y0 - 3.8f * bspline_offset_y,
        y0,
    };
    BSpline l_top = 
    {
        .x_coeffs = l_top_x,
        .y_coeffs = l_top_y,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };
    bspline_modify(c, &l_top, sb, opts, modify);

    float l_bottom_x[6] = 
    {
        x0, 
        x0 + bspline_offset_x,
        x0 + 2 * bspline_offset_x,
        x0 + 3 * bspline_offset_x,
    };

    float l_bottom_y[6] = 
    {
        y0, 
        y0 + bspline_offset_y,
        y0 + 3.8f * bspline_offset_y,
        y0,
    };
    BSpline l_bottom = 
    {
        .x_coeffs = l_bottom_x,
        .y_coeffs = l_bottom_y,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };
    bspline_modify(c, &l_bottom, sb, opts, modify);

    float internal_x[6] = 
    {
        x0 + 2 * bspline_offset_x, 
        x0 +  bspline_offset_x,
        x0 +  bspline_offset_x,
        x0 + 2 * bspline_offset_x,
    };

    float internal_y[6] = 
    {
        y0 - 2 * bspline_offset_y, 
        y0,
        y0 + bspline_offset_y,
        y0 + 2 * bspline_offset_y,
    };
    BSpline internal = 
    {
        .x_coeffs = internal_x,
        .y_coeffs = internal_y,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };
    bspline_modify(c, &internal, sb, opts, modify);

}

void bspline_width_r_eye(
        Canvas* c, SimpleBrush* sb, int x0, int y0, void* opts,
        void(*modify)
        (Canvas* c, 
         BSpline* bsp, float u, Line* l,  SimpleBrush* sb, void* opts))
{

    int bspline_offset_x = 10;
    int bspline_offset_y = 5;
    float knots[14] = {
        0,
        0,
        0,
        0,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
    };


    float l_top_x[6] = 
    {
        x0, 
        x0 - bspline_offset_x,
        x0 - 2 * bspline_offset_x,
        x0 - 3 * bspline_offset_x,
    };

    float l_top_y[6] = 
    {
        y0, 
        y0 - bspline_offset_y,
        y0 - 3.8f * bspline_offset_y,
        y0,
    };
    BSpline l_top = 
    {
        .x_coeffs = l_top_x,
        .y_coeffs = l_top_y,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };
    bspline_modify(c, &l_top, sb, opts, modify);

    float l_bottom_x[6] = 
    {
        x0, 
        x0 - bspline_offset_x,
        x0 - 2 * bspline_offset_x,
        x0 - 3 * bspline_offset_x,
    };

    float l_bottom_y[6] = 
    {
        y0, 
        y0 + bspline_offset_y,
        y0 + 3.8f * bspline_offset_y,
        y0,
    };
    BSpline l_bottom = 
    {
        .x_coeffs = l_bottom_x,
        .y_coeffs = l_bottom_y,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };  

    bspline_modify(c, &l_bottom, sb, opts, modify);
    float internal_x[6] = 
    {
        x0 - 2 * bspline_offset_x, 
        x0 -  bspline_offset_x,
        x0 -  bspline_offset_x,
        x0 - 2 * bspline_offset_x,
    };

    float internal_y[6] = 
    {
        y0 - 2 * bspline_offset_y, 
        y0,
        y0 + bspline_offset_y,
        y0 + 2 * bspline_offset_y,
    };
    BSpline internal = 
    {
        .x_coeffs = internal_x,
        .y_coeffs = internal_y,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };
    bspline_modify(c, &internal, sb, opts, modify);

}
    
void bspline_width_nose(
        Canvas* c, SimpleBrush* sb, int x0, int y0, void* opts,
        void(*modify)
        (Canvas* c, 
         BSpline* bsp, float u, Line* l,  SimpleBrush* sb, void* opts))
{


    int bspline_offset_x = 3;
    int bspline_offset_y = 2;
    float knots[14] = {
        0,
        0,
        0,
        0,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
    };

    float l_top_x[6] = 
    {
        x0, 
        x0 + bspline_offset_x,
        x0 + 2 * bspline_offset_x,
        x0 + 3 * bspline_offset_x,
    };

    float l_top_y[6] = 
    {
        y0, 
        y0 - bspline_offset_y,
        y0 - 2 * bspline_offset_y,
        y0 - 3 * bspline_offset_y,
    };
    BSpline l_top = 
    {
        .x_coeffs = l_top_x,
        .y_coeffs = l_top_y,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };
    bspline_modify(c, &l_top, sb, opts, modify);

    int r_x_off = 15;
    float r_top_x[6] = 
    {
        x0, 
        x0 - 0.5f * bspline_offset_x,
        x0 - 1 * bspline_offset_x,
        x0 - 1.5f * bspline_offset_x,
    };

    for (int i = 0; i < 4; i++)
    {
        r_top_x[i] += r_x_off;
    }

    float r_top_y[6] = 
    {
        y0, 
        y0 - bspline_offset_y,
        y0 - 2 * bspline_offset_y,
        y0 - 3 * bspline_offset_y,
    };
    BSpline r_top = 
    {
        .x_coeffs = r_top_x,
        .y_coeffs = r_top_y,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };

    bspline_modify(c, &r_top, sb, opts, modify);

    int b_offset_y = 2;

    float l_bottom_x[6] = 
    {
        x0, 
        x0 + bspline_offset_x,
        x0 + 2 * bspline_offset_x,
        x0 + 3 * bspline_offset_x,
    };

    float l_bottom_y[6] = 
    {
        y0, 
        y0 + bspline_offset_y,
        y0 + 2 * bspline_offset_y,
        y0 + 3 * bspline_offset_y,
    };

    for (int i = 0; i < 4; i++)
    {
        l_bottom_y[i] += b_offset_y;
    }
    BSpline l_bottom = 
    {
        .x_coeffs = l_bottom_x,
        .y_coeffs = l_bottom_y,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };
    bspline_modify(c, &l_bottom, sb, opts, modify);

    float r_bottom_x[6] = 
    {
        x0, 
        x0 - 0.5f * bspline_offset_x,
        x0 - 1 * bspline_offset_x,
        x0 - 1.5f * bspline_offset_x,
    };

    for (int i = 0; i < 4; i++)
    {
        r_bottom_x[i] += r_x_off;
    }

    float r_bottom_y[6] = 
    {
        y0, 
        y0 + bspline_offset_y,
        y0 + 2 * bspline_offset_y,
        y0 + 3 * bspline_offset_y,
    };

    for (int i = 0; i < 4; i++)
    {
        r_bottom_y[i] += b_offset_y;
    }
    BSpline r_bottom = 
    {
        .x_coeffs = r_bottom_x,
        .y_coeffs = r_bottom_y,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };
    bspline_modify(c, &r_bottom, sb, opts, modify);

}

void bspline_width_smile(
        Canvas* c, SimpleBrush* sb, int x0, int y0, void* opts,
        void(*modify)
        (Canvas* c, 
         BSpline* bsp, float u, Line* l,  SimpleBrush* sb, void* opts))
{

    int bspline_offset_x = 20;
    int bspline_offset_y = 10;
    float knots[14] = {
        0,
        0,
        0,
        0,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
    };

    float smile_x[6] = 
    {
        x0, 
        x0 + bspline_offset_x,
        x0 + 2 * bspline_offset_x,
        x0 + 3 * bspline_offset_x,
    };

    float smile_y[6] = 
    {
        y0, 
        y0 + bspline_offset_y,
        y0 +  bspline_offset_y,
        y0,
    };
    BSpline smile = 
    {
        .x_coeffs = smile_x,
        .y_coeffs = smile_y,
        .coeffs_count= 4,
        .seg_count = 1e3,
        .knots = knots,
        .knot_count = 8,
        .order = 4,
    };
    bspline_modify(c, &smile, sb, opts, modify);
}
