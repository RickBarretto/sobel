#include <app/filters.h>

void extended_sobel(Bitmap *bmp, PinIO pins) {

    int8_t x_mask[5][5] = {
        {-1, -2, -1,  0,  0},
        {-2, -4, -2,  0,  0},
        { 0,  0,  0,  0,  0},
        { 2,  4,  2,  0,  0},
        { 1,  2,  1,  0,  0}
    };
    
    int8_t y_mask[5][5] = {
        {-1, -2,  0,  2,  1},
        {-2, -4,  0,  4,  2},
        {-1, -2,  0,  2,  1},
        { 0,  0,  0,  0,  0},
        { 0,  0,  0,  0,  0}
    };


    byte_t *filtered = malloc(bmp->bounds.image);
    if (!filtered) {
        perror("Error: could not allocate memory for filtered image.\n");
        return;
    }

    for (size_t i = 0; i < bmp->info.height; i++) {
        size_t row = bmp->info.height - i - 1;

        for (size_t j = 0; j < bmp->info.width; j++) {
            size_t position = row * bmp->bounds.row + j * (bmp->info.depth / 8);

            #ifdef IN_PROD
                uint8_t neighborhood[5][5];
                sub_matrix(bmp, i, j, neighborhood);
                int gx = convolution(pins, neighborhood, x_mask);
                int gy = convolution(pins, neighborhood, y_mask);
            #else
                int gx = convolution(bmp, i, j, x_mask);
                int gy = convolution(bmp, i, j, y_mask);
            #endif
            
            int magnitude = round(sqrt((gx * gx + gy * gy)));
            if (magnitude > 255) {
                magnitude = 255;
            }

            uint8_t pixel = magnitude;
            filtered[position + 0] = pixel;
            filtered[position + 1] = pixel;
            filtered[position + 2] = pixel;
        }
    }

    free(bmp->data);
    bmp->data = filtered;
}