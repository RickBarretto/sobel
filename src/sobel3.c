#include <app/filters.h>

static int convolution(
    Bitmap *bitmap,
    uint32_t pixel_row,
    uint32_t pixel_col,
    int8_t kernel[5][5]
) {

    int sum = 0;

    for (int kernel_row = 0; kernel_row < 5; kernel_row++) {
        for (int kernel_col = 0; kernel_col < 5; kernel_col++) {
            int32_t image_row = pixel_row + kernel_row - 2;
            int32_t image_col = pixel_col + kernel_col - 2;

            // Skips out-of-bounds pixels
            if (image_row < 0)                      continue; 
            if (image_row >= bitmap->info.height)   continue;
            if (image_col < 0)                      continue; 
            if (image_col >= bitmap->info.width)    continue;

            uint32_t mirrored_row = bitmap->info.height - image_row - 1;
            size_t pixel_index = (mirrored_row * bitmap->bounds.row) 
                + (image_col * (bitmap->info.depth / 8));
            uint8_t grayscale_value = bitmap->data[pixel_index];

            sum += kernel[kernel_row][kernel_col] * grayscale_value;
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