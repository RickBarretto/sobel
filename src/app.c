#include <stdio.h>

#include <app/types.h>
#include <app/cli.h>

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

    printf("Got:\t%s\n", cli->input_file);
    printf("To:\t%s\n", cli->output_file);

    printf("USE_LAPLACE_MPU_V2=%d\n", USE_LAPLACE_MPU_V2);

    cli_free(cli);

}