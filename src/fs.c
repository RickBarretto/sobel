#include <app/fs.h>


byte_t * fs_read_bytes(FILE * file, size_t size, size_t amount) {
    if (file == NULL || size == 0 || amount == 0)
        return NULL;

    byte_t * buffer = malloc(size * amount);
    if (buffer == NULL) 
        return NULL;

    size_t read = fread(buffer, size, amount, file);
    if (read != amount) {
        free(buffer);
        return NULL;
    }

    return buffer;
}


bool skip_offset(FILE * file, size_t offset) {
    if (fseek(file, offset, SEEK_SET) != 0) {
        return false;
    }

    return true;
}


byte_t * fs_read_bytes_from(FILE * file, size_t offset, size_t size) {
    unless (file) 
        return NULL;

    if (offset < 0 || size <= 0)
        return NULL;

    unless (skip_offset(file, offset))
        return NULL;
    
    return fs_read_bytes(file, sizeof(byte_t), size);
}

bool fs_write(FILE * file, byte_t * data, size_t type_size, size_t amount) {
    return fwrite(data, type_size, amount, file) == amount;
}

bool file_exists(const cstring_t filename) {
    FILE * file = fopen(filename, "r");
    if (file) fclose(file);
    return (file != NULL);
}
