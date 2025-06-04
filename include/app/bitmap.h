#ifndef APP_BITMAP_H
#define APP_BITMAP_H

#include <app/types.h>


typedef struct {
    byte_t signature[2];
    uint32_t file_size;
    uint32_t offset;
} BmpHeader;

typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t depth;
} BmpInfo;


typedef struct {
    size_t row;
    size_t image;
} ByteBounds;

typedef struct {
    BmpHeader header;
    BmpInfo info;
    ByteBounds bounds;
    byte_t *raw_header;
    byte_t *data;
} Bitmap;



Bitmap * bmp_read(cstring_t filename);
void bmp_write(const cstring_t filename, Bitmap * bmp);


#endif