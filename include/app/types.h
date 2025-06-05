#ifndef FRONTIER_TYPES_H
#define FRONTIER_TYPES_H

#include <iso646.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define unless(x) if (not(x))

typedef char* cstring_t;
typedef int exit_t;
typedef uint8_t byte_t;

cstring_t cstr_copy(const char const * src);

#endif