#ifndef APP_FS_H
#define APP_FS_H

#include <stdio.h>
#include <stdlib.h>

#include <app/types.h>

#define FS_READ_BYTES "rb"
#define FS_WRITE_BYTES "wb"


#define with_open(file, filename, mode)             \
    for (FILE * file = fopen((filename), (mode));   \
         file != NULL;                              \
         fclose(file), file = NULL)

byte_t * fs_read_bytes(FILE *file, size_t size, size_t amount);
byte_t * fs_read_bytes_from(FILE * file, size_t offset, size_t size);

bool fs_write(FILE * file, byte_t * data, size_t type_size, size_t amount);


#endif