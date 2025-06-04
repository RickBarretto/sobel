#ifndef MPU_LAPLACE_H
#define MPU_LAPLACE_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>
#include <stdio.h>


/// HPS lightweight bridge base address & span
#define LW_BRIDGE_BASE   0xFF200000u
#define LW_BRIDGE_SPAN   0x00005000u


/// PIO register offsets
#define PIO_CMD_OFFSET   0x00u
#define PIO_STAT_OFFSET  0x10u


/// Matrix dimensions
#define DIM    5
#define N_BYTES (DIM * DIM)
#define N_BITS   (N_BYTES * 8u)


/// @brief  5x5 Matrix type
typedef uint8_t Matrix[DIM][DIM];


/// Pin I/O to do the communication
typedef struct {
    volatile uint32_t *cmd;
    volatile uint32_t *stat;
} PinIO;


/// @brief HPS-FPGA Connection's file mapping
typedef struct {
    int   fd;
    void *base;
} Connection;


/// @brief  Encoded instruction
typedef struct {

    /// @brief  Operation code
    unsigned opcode;

    /// @brief  Matrix size to be used in determinant operations.
    ///         Default value: 0.
    unsigned matrix_size;

    /// @brief  Pre-built upper-bits of the word
    uint32_t base_cmd;

} Instruction;

// ─── Library API ──────────────────────────────────────────────────────────


/// @brief Wait for ~1µs
extern void mpu_delay_1us(void);

/// @brief open /dev/mem & mmap the LW bridge
/// @return HPS-FPGA Connection
extern Connection mpu_new_connection(void);
/// @brief    Unmap and close HPS-FPGA connection
/// @param c  Connection
extern void mpu_close_connection(Connection *c);


/// @brief Build the high bits of the command word
/// @param opcode      Operation code 
/// @param matrix_size Matrix's size
/// @return            1:1 instruction code to be sent to FPGA.
extern uint32_t mpu_build_base_cmd(
    unsigned opcode,
    unsigned matrix_size
);


/// @brief Advance to next stage
/// @param pins     I/O pins
/// @param base_cmd Base command
///
/// Sent a signal to the co-processor that triggers the next stage.
extern void mpu_next_stage(PinIO pins, uint32_t base_cmd);


/// @brief Send all bits of a 5×5 matrix
/// @param M        Matrix
/// @param pins     I/O Pins
/// @param base_cmd Base command
extern void mpu_store(Matrix M, PinIO pins, uint32_t base_cmd);

/* read back N_BYTES into R[row][col] */


/// @brief Read the 5x5 matrix result from co-processor
/// @param R        Matrix that will get the result
/// @param pins     I/O pins
/// @param base_cmd Base command
extern void mpu_load(Matrix R, PinIO pins, uint32_t base_cmd);


/// @brief Default test matrices A, B and empty Result.
/// @param A Matrix A
/// @param B Matrix A
/// @param R Result Matrix
void mpu_init_default_matrices(Matrix A,
                                      Matrix B,
                                      Matrix R)
{
    int r, c;
    for (r = 0; r < DIM; r++) {
        for (c = 0; c < DIM; c++) {
            A[r][c] = (uint8_t)(r * DIM + c + 1);
            B[r][c] = (uint8_t)(DIM*DIM + 1 - A[r][c]);
            R[r][c] = 0;
        }
    }
}

#endif