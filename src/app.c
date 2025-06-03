#include <stdio.h>

#include <app/types.h>
#include <app/cli.h>

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

    printf("Got:\t%s\n", cli->input_file);
    printf("To:\t%s\n", cli->output_file);

    cli_free(cli);

}