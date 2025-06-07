#ifndef APP_FILTER_h
#define APP_FILTER_h

#include <math.h>
#include <stdio.h>

#include <app/matrix.h>
#include <app/bitmap.h>

void preprocess(Bitmap *bmp);
void postprocess(Bitmap *bmp);
void filter_xy(Bitmap *bmp, PinIO pins, int8_t x_mask[5][5], int8_t y_mask[5][5]);

void sobel(Bitmap *bmp, PinIO pins);
void prewitt(Bitmap *bmp, PinIO pins);
void roberts(Bitmap *bmp, PinIO pins);
void laplacian(Bitmap *bmp, PinIO pins);
void extended_sobel(Bitmap *bmp, PinIO pins);

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