
#include <app/filters.h>

int convolution(
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


int execute(
    PinIO pins, 
    Matrix matrix_a, 
    Matrix matrix_b
) {
    Matrix result = {0};
    #ifdef IN_PROD
    instruction.base_cmd = mpu_build_base_cmd(1, 5);

    mpu_next_stage(pins, instruction.base_cmd);
    mpu_store(matrix_a, pins, instruction.base_cmd);
    mpu_store(matrix_b, pins, instruction.base_cmd);
    mpu_next_stage(pins, instruction.base_cmd);
    mpu_load(result, pins, instruction.base_cmd);
    #endif

    return result[4][4];
}


 void sub_matrix(Bitmap *bmp, size_t center_row, size_t center_col, uint8_t matrix[5][5]) {
    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            int row = center_row + i;
            int col = center_col + j;
            
            if (row < 0) row = 0;
            if (row >= bmp->info.height) row = bmp->info.height - 1;
            if (col < 0) col = 0;
            if (col >= bmp->info.width) col = bmp->info.width - 1;
            
            size_t bitmap_row = bmp->info.height - row - 1;
            size_t position = bitmap_row * bmp->bounds.row + col * (bmp->info.depth / 8);
            
            matrix[i + 2][j + 2] = bmp->data[position];
        }
    }
}