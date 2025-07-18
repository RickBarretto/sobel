#include <app/filters.h>

void extended_sobel(Bitmap *bmp, PinIO pins) {

    uint8_t x_mask[5][5] = {
        {-1, -2, -1,  0,  0},
        {-2, -4, -2,  0,  0},
        { 0,  0,  0,  0,  0},
        { 2,  4,  2,  0,  0},
        { 1,  2,  1,  0,  0}
    };
    
    uint8_t y_mask[5][5] = {
        {-1, -2,  0,  2,  1},
        {-2, -4,  0,  4,  2},
        {-1, -2,  0,  2,  1},
        { 0,  0,  0,  0,  0},
        { 0,  0,  0,  0,  0}
    };

    filter_xy(bmp, pins, x_mask, y_mask);

}