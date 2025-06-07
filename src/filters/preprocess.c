#include <app/filters.h>

void preprocess(Bitmap *bmp) {

    byte_t *filtered = malloc(bmp->bounds.image);
    if (!filtered) {
        perror("Error: could not allocate memory for filtered image.\n");
        return;
    }

    for (size_t i = 0; i < bmp->info.height; i++) {
        size_t row = bmp->info.height - i - 1;

        for (size_t j = 0; j < bmp->info.width; j++) {
            size_t position = row * bmp->bounds.row + j * (bmp->info.depth / 8);
            filtered[position + 0] = filtered[position + 0] << 2;
            filtered[position + 1] = filtered[position + 1] << 2;
            filtered[position + 2] = filtered[position + 2] << 2;
        }
    }

    free(bmp->data);
    bmp->data = filtered;
}

void postprocess(Bitmap *bmp) {

    byte_t *filtered = malloc(bmp->bounds.image);
    if (!filtered) {
        perror("Error: could not allocate memory for filtered image.\n");
        return;
    }

    for (size_t i = 0; i < bmp->info.height; i++) {
        size_t row = bmp->info.height - i - 1;

        for (size_t j = 0; j < bmp->info.width; j++) {
            size_t position = row * bmp->bounds.row + j * (bmp->info.depth / 8);
            filtered[position + 0] = filtered[position + 0] >> 2;
            filtered[position + 1] = filtered[position + 1] >> 2;
            filtered[position + 2] = filtered[position + 2] >> 2;
        }
    }

    free(bmp->data);
    bmp->data = filtered;
}