#ifndef APP_CLI_H
#define APP_CLI_H

#include <app/types.h>

/// bmp_t << cstring_t : Bitmap's Path
typedef cstring_t bmp_t;


/// Command Line Interface representation.
///
/// Application's CLI has two positional arguments: `<input>` and `<output>`.
/// Each one is a Path to a BitMap image.
typedef struct {
    cstring_t app;
    cstring_t description;
    cstring_t usage;

    // Positional Arguments
    bmp_t input_file;
    bmp_t output_file;
} Cli;


/// Create a CLI object from command-line.
///
/// Params
/// ------
/// app: cstring_t
///    Application name to be displayed. 
/// description: cstring_t
///    Application's description. 
/// arguments: cstring_t[] 
///     Values from CLI. example: ["input.bpm", "output.bmp"]
/// count: size_t
///     Arguments count. exammple: 2
Cli * cli_from(cstring_t app, cstring_t description, cstring_t arguments[], size_t count);


/// Free CLI and its fields from memory.
void cli_free(Cli* cli);


#endif