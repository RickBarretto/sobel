#include <stdio.h>

#include <app/types.h>
#include <app/cli.h>
#include <app/bitmap.h>
#include <app/filters.h>

#include <app/matrix.h>

#define APP_NAME "sobel"
#define APP_DESCRIPTION "Apply border filters to bitmap images."


int main(
    int argc, 
    char* argv[]
) {

    Cli * cli = cli_from(
        APP_NAME, 
        APP_DESCRIPTION, 
        argv, 
        argc
    );

    Bitmap * image = bmp_read(cli->input_file);

    with_connection(bridge) {
        PinIO pins = {
            .cmd  = bridge.connection.base + PIO_CMD_OFFSET,
            .stat = bridge.connection.base + PIO_STAT_OFFSET
        };

        switch (cli->filter) {
        case Sobel: 
            sobel(image, pins); 
            break;
        case ExtendedSobel: 
            extended_sobel(image, pins); 
            break;
        case Prewitt: 
            prewitt(image, pins); 
            break;
        case Roberts: 
            roberts(image, pins); 
            break;
        case Laplacian: 
            laplacian(image, pins); 
            break;
        }
    }

    bmp_write(cli->output_file, image);
    cli_free(cli);

}