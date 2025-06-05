#include <app/filters.h>

void sobel(Bitmap *bmp, PinIO pins) {

    int8_t x_mask[5][5] = {
        {-1, -2, -1,  0,  0},
        { 0,  0,  0,  0,  0},
        { 1,  2,  1,  0,  0},
        { 0,  0,  0,  0,  0},
        { 0,  0,  0,  0,  0}
    };
    
    int8_t y_mask[5][5] = {
        {-1,  0,  1,  0,  0},
        {-2,  0,  2,  0,  0},
        {-1,  0,  1,  0,  0},
        { 0,  0,  0,  0,  0},
        { 0,  0,  0,  0,  0}
    };

    filter_xy(bmp, pins, x_mask, y_mask);

}