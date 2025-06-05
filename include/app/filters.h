#ifndef APP_FILTER_h
#define APP_FILTER_h

#include <math.h>
#include <stdio.h>

#include <app/matrix.h>
#include <app/bitmap.h>



void sobel3(Bitmap *bmp, PinIO pins);
void prewitt(Bitmap *bmp, PinIO pins);

int convolution(
    Bitmap *bitmap,
    uint32_t pixel_row,
    uint32_t pixel_col,
    int8_t kernel[5][5]
);

int execute(
    PinIO pins, 
    Matrix matrix_a, 
    Matrix matrix_b
);

void sub_matrix(Bitmap *bmp, size_t center_row, size_t center_col, uint8_t matrix[5][5]);

#endif