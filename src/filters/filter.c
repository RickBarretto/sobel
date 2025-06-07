#include <math.h>

#include <app/filters.h>

void filter_xy(Bitmap *bmp, PinIO pins, int8_t x_mask[5][5], int8_t y_mask[5][5]) {

    byte_t *filtered = malloc(bmp->bounds.image);
    if (!filtered) {
        perror("Error: could not allocate memory for filtered image.\n");
        return;
    }

    for (size_t i = 0; i < bmp->info.height; i++) {
        size_t row = bmp->info.height - i - 1;
        printf("%d\n", i);

        for (size_t j = 0; j < bmp->info.width; j++) {
            size_t position = row * bmp->bounds.row + j * (bmp->info.depth / 8);

            int8_t neighborhood[5][5];
            sub_matrix(bmp, i, j, neighborhood);
            int gx = execute(pins, neighborhood, x_mask);
            int gy = execute(pins, neighborhood, y_mask);
            
            int magnitude = round(sqrt((gx * gx + gy * gy)));
            if (magnitude > 255) {
                magnitude = 255;
            }

            int8_t pixel = magnitude * 1.2;
            filtered[position + 0] = pixel;
            filtered[position + 1] = pixel;
            filtered[position + 2] = pixel;
        }
    }

    free(bmp->data);
    bmp->data = filtered;
}