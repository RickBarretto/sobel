#include <app/filters.h>

static int convolution(
    Bitmap * bmp,
    uint32_t i,
    uint32_t j,
    int8_t mask[5][5]
) {
    int sum = 0;

    for (int k = 0; k < 5; k++) {
        for (int l = 0; l < 5; l++) {
            int di = k - 2;
            int dj = l - 2;
            int32_t ni = i + di;
            int32_t nj = j + dj;
            if (ni < 0 || ni >= bmp->info.height || nj < 0 || nj >= bmp->info.width) {
                continue;
            }
            uint32_t row = bmp->info.height - ni - 1;
            size_t neighbor_position = row * bmp->bounds.row + nj * (bmp->info.depth / 8);
            uint8_t gray_value = bmp->data[neighbor_position];
            sum += mask[k][l] * gray_value;
        }
    }
    return sum;
}

void sobel3(Bitmap *bmp) {

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


    byte_t *filtered = malloc(bmp->bounds.image);
    if (!filtered) {
        fprintf(stderr, "Error: could not allocate memory for filtered image.\n");
        return;
    }

    for (size_t i = 0; i < bmp->info.height; i++) {
        size_t row = bmp->info.height - i - 1;

        for (size_t j = 0; j < bmp->info.width; j++) {
            size_t position = row * bmp->bounds.row + j * (bmp->info.depth / 8);

            int gx = convolution(bmp, i, j, x_mask);
            int gy = convolution(bmp, i, j, y_mask);

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