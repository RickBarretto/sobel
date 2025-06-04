#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include <app/fs.h>
#include <app/bitmap.h>

#define BMP_FILE_HEADER_SIZE 14
#define BMP_INFO_HEADER_SIZE 40

#define BMP_SIGNATURE_OFFSET 0
#define BMP_FILE_SIZE_OFFSET 2
#define BMP_DATA_OFFSET_OFFSET 10

#define BMP_WIDTH_OFFSET 4
#define BMP_HEIGHT_OFFSET 8
#define BMP_DEPTH_OFFSET 14


bool byte_equal(const byte_t * a, const byte_t * b, size_t offset) {
    return memcmp(a, b, offset) == 0;
}


size_t int_from_bytes(byte_t * data, size_t amount) {
    size_t result = 0;
    memcpy(&result, data, amount);
    return result;
}


size_t row_byte_bound(uint32_t width, uint32_t depth) {
    return (width * (depth / 8) + 3) & (~3);
}


size_t image_byte_bound(size_t row_bytes, uint32_t height) {
    return row_bytes * height;
}


ByteBounds bounds_from(BmpInfo info) {
    size_t row = row_byte_bound(info.width, info.depth);
    ByteBounds bound = {
        .row = row,
        .image = image_byte_bound(row, info.height)
    };
    return bound;
}


Bitmap * bmp_read(const cstring_t filename) {
    Bitmap *bmp = malloc(sizeof(Bitmap));
    if (!bmp) {
        perror("Failed to allocate memory for Bitmap");
        exit(EXIT_FAILURE);
    }

    with_open(file, filename, FS_READ_BYTES) {
        byte_t *file_header = fs_read_bytes(file, sizeof(byte_t), BMP_FILE_HEADER_SIZE);
        byte_t *info_header = fs_read_bytes(file, sizeof(byte_t), BMP_INFO_HEADER_SIZE);

        if (!byte_equal(file_header, "BM", 2)) {
            fprintf(stderr, "Invalid BMP format.\n");
            exit(EXIT_FAILURE);
        }

        bmp->header.signature[0] = file_header[0];
        bmp->header.signature[1] = file_header[1];

        bmp->header.file_size = int_from_bytes(&file_header[BMP_FILE_SIZE_OFFSET], 4);
        bmp->header.offset = int_from_bytes(&file_header[BMP_DATA_OFFSET_OFFSET], 4);

        bmp->info.width = int_from_bytes(&info_header[BMP_WIDTH_OFFSET], 4);
        bmp->info.height = int_from_bytes(&info_header[BMP_HEIGHT_OFFSET], 4);
        bmp->info.depth = int_from_bytes(&info_header[BMP_DEPTH_OFFSET], 2);

        bmp->bounds = bounds_from(bmp->info);

        bmp->raw_header = malloc(BMP_FILE_HEADER_SIZE + BMP_INFO_HEADER_SIZE);
        memcpy(bmp->raw_header, file_header, BMP_FILE_HEADER_SIZE);
        memcpy(bmp->raw_header + BMP_FILE_HEADER_SIZE, info_header, BMP_INFO_HEADER_SIZE);

        bmp->data = fs_read_bytes_from(file, bmp->header.offset,
                                       sizeof(byte_t) * bmp->bounds.image);

        free(file_header);
        free(info_header);
    }

    return bmp;
}

void bmp_write(const cstring_t filename, Bitmap * bmp) {
    with_open(file, filename, FS_WRITE_BYTES) {
        unless(file) {
            fprintf(stderr, "Cannot generate output bitmap.\n");
            exit(EXIT_FAILURE);
        }

        fs_write(file, bmp->raw_header, sizeof(byte_t),
                 BMP_FILE_HEADER_SIZE + BMP_INFO_HEADER_SIZE);
        fs_write(file, bmp->data, sizeof(byte_t), bmp->bounds.image);
    }
}

void bmp_free(Bitmap * bmp) {
    if (bmp) {
        free(bmp->raw_header);
        free(bmp->data);
        free(bmp);
    }
}
