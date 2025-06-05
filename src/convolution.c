
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
    Matrix matrix_b, 
) {
    Matrix result = {0};
    instruction.base_cmd = mpu_build_base_cmd(1, 5);

    mpu_next_stage(pins, instruction.base_cmd);
    mpu_store(matrix_a, pins, instruction.base_cmd);
    mpu_store(matrix_b, pins, instruction.base_cmd);
    mpu_next_stage(pins, instruction.base_cmd);
    mpu_load(result, pins, instruction.base_cmd);

    return result[4][4];
}