#include <stdio.h>

#include <app/types.h>
#include <app/cli.h>
#include <app/bitmap.h>

#define APP_NAME "sobel"
#define APP_DESCRIPTION "Apply border filters to bitmap images."

#ifdef IN_PROD
    #define USE_LAPLACE_MPU_V2 true
#else
    #define USE_LAPLACE_MPU_V2 false
#endif

exit_t main(
    size_t arguments_length, 
    cstring_t arguments[]
) {

    Cli * cli = cli_from(
        APP_NAME, 
        APP_DESCRIPTION, 
        arguments, 
        arguments_length
    );

    Bitmap * image = bmp_read(cli->input_file);
    bmp_write(cli->output_file, image);

    cli_free(cli);

}