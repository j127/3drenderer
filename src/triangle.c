#include "triangle.h"
#include "display.h"

void int_swap(int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

// Draw a filled triangle with a flat bottom.
//
// Slope 1 is on the left, slope 2 is on the right. (x2,y2) is (Mx,My).
//
//          (x0, y0)
//             /\
//            /  \
//           /    \
//          /      \
//         /        \
//    (x1,y1)------(x2,y2)
//
void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2,
                               uint32_t color) {
    float inverted_slope_1 = (float)(x1 - x0) / (y1 - y0);
    float inverted_slope_2 = (float)(x2 - x0) / (y2 - y0);

    // Start x_start and x_end from the top vertex (x0,y0)
    float x_start = x0;
    float x_end = x0;

    // Loop over the scanlines from top to bottom
    for (int y = y0; y <= y2; y++) {
        draw_line(x_start, y, x_end, y, color);
        x_start += inverted_slope_1;
        x_end += inverted_slope_2;
    }
}

// Draw a filled triangle with a flat top.
//
// (x1,y1) is (Mx,My) here.
//
//    (x0,y0)------(x1,y1)
//        \_          \
//           \_        \
//              \_      \
//                 \_    \
//                   \    \
//                     \_  \
//                        \_\
//                           \
//                        (x2,y2)
//
void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2,
                            uint32_t color) {
    float inverted_slope_1 = (float)(x2 - x0) / (y2 - y0);
    float inverted_slope_2 = (float)(x2 - x1) / (y2 - y1);

    // Start x_start and x_end from the bottom vertex (x2,y2)
    float x_start = x2;
    float x_end = x2;

    // Loop over the scanlines from bottom to top
    for (int y = y2; y >= y0; y--) {
        draw_line(x_start, y, x_end, y, color);
        x_start -= inverted_slope_1;
        x_end -= inverted_slope_2;
    }
}

// Draw a filled triangle with the flat-top/flat-bottom method.
//
//          (x0, y0)
//             /\
//            /  \
//           /    \
//          /      \
//         /        \
//    (x1,y1)------(Mx,My)
//        \_          \
//           \_        \
//              \_      \
//                 \_    \
//                   \    \
//                     \_  \
//                        \_\
//                           \
//                        (x2,y2)
//
void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2,
                          uint32_t color) {
    // Sort the vertices by y-coordinate, ascending (y0 < y1 < y2)
    if (y0 > y1) {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
    }

    if (y1 > y2) {
        int_swap(&y1, &y2);
        int_swap(&x1, &x2);
    }

    if (y0 > y1) {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
    }

    // Calculate the new vertex (Mx,My) using triangle similarity
    //
    // Mx - x0   y1 - y0
    // ------- = -------
    // x2 - x0   y2 - y0
    //
    // or:
    //
    // (Mx - x0)(y2 - y0) = (x2 - x0)(y1 - y0)
    //
    // Gives the formula:
    //
    //      (x2 - x0)(y1 - y0)
    // Mx = ------------------ + x0
    //           (y2 - y0)
    int My = y1;
    int Mx = (((x2 - x0) * (y1 - y0)) / (y2 - y0)) + x0;

    // Draw flat-bottom triangle
    fill_flat_bottom_triangle(x0, y0, x1, y1, Mx, My, color);

    // Draw flat-top triangle
    fill_flat_top_triangle(x1, y1, Mx, My, x2, y2, color);
}
