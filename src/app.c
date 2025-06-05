#ifdef IN_PROD
    #define USE_LAPLACE_MPU_V2 true
#else
    #define USE_LAPLACE_MPU_V2 false
#endif

#include <stdio.h>

#include <app/types.h>
#include <app/cli.h>
#include <app/bitmap.h>
#include <app/filters.h>

#include <app/matrix.h>

#define APP_NAME "sobel"
#define APP_DESCRIPTION "Apply border filters to bitmap images."


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

    with_connection(bridge) {
        PinIO pins = {
            .cmd  = bridge.connection.base + PIO_CMD_OFFSET,
            .stat = bridge.connection.base + PIO_STAT_OFFSET
        };

        laplacian(image, pins);
    }

    bmp_write(cli->output_file, image);

    cli_free(cli);

}