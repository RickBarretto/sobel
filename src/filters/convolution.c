
#include <app/filters.h>
#include <mpu/mpu.h>
#include <stdio.h>

int execute(
    PinIO pins, 
    Matrix matrix_a, 
    Matrix matrix_b
) {
    Matrix result = {0};
    Instruction instruction = {0};
    instruction.base_cmd = mpu_build_base_cmd(1, 5);

    mpu_next_stage(pins, instruction.base_cmd);
    mpu_store(matrix_a, pins, instruction.base_cmd);
    mpu_store(matrix_b, pins, instruction.base_cmd);
    mpu_next_stage(pins, instruction.base_cmd);
    mpu_load(result, pins, instruction.base_cmd);

    
    unsigned int value = result[0][0];
    
    return value;
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
            
            matrix[i + 2][j + 2] = bmp->data[position] ;
        }
    }
}