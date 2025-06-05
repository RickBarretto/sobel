#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <app/cli.h>


// =~=~=~=~=~=~=~=~ General ~=~=~=~=~=~=~=~= 


Cli * cli_new(cstring_t app, cstring_t description, cstring_t usage) {
    Cli * cli = malloc(sizeof(Cli));

    cli->app = app;
    cli->description = description;
    cli->usage = usage;
    cli->input_file = "";
    cli->output_file = "";

    return cli;
}


// =~=~=~=~=~=~=~=~ Display ~=~=~=~=~=~=~=~= 

void cli_print_section(cstring_t title) {
    printf("\n%s:\n", title);
}

void cli_option(cstring_t line) {
    printf("    %s\n", line);
}

void cli_example(Cli * cli, cstring_t code) {
    printf("    $ %s %s\n\n", cli->app, code);
}

void cli_help(Cli* cli) {
    printf("Usage: %s %s\n", cli->app, cli->usage);
    printf("%s\n", cli->description);

    cli_print_section("Options");
    cli_option("--help | -h       displays this.");

    cli_print_section("Example");
    cli_example(cli, "input.bmp output.bmp");

    cli_free(cli);
    exit(EXIT_SUCCESS);
}


void cli_panic(Cli* cli, cstring_t error) {
    fprintf(stderr, "%s: %s\n", cli->app, error);
    printf("Try '%s --help' for more information.\n", cli->app);

    cli_free(cli);
    exit(EXIT_FAILURE);
}


// =~=~=~=~=~=~=~=~ Validation ~=~=~=~=~=~=~=~= 


bool cli_str_equals(cstring_t a, cstring_t b) {
    return strcmp(a, b) == 0;
}

bool cli_is_help(cstring_t argument) {
    return cli_str_equals(argument, "--help")
        || cli_str_equals(argument, "-h");
}


bool cli_is_bmp(cstring_t filename) {
    size_t len = strlen(filename);
    if (len < 4) return false;

    return cli_str_equals(filename + len - 4, ".bmp");
}


// =~=~=~=~=~=~=~=~ Public ~=~=~=~=~=~=~=~= 


Cli *cli_from(cstring_t app, cstring_t description, cstring_t arguments[], size_t count) {
    cstring_t usage = "<bitmap input> <bitmap output>";
    Cli* cli = cli_new(app, description, usage);

    switch (count) {
    case 3: {
        // Example: app input.bmp output.bmp 
        cli->input_file = arguments[1];
        cli->output_file = arguments[2];
        break;
    }
    case 1: {
        cli_panic(cli, "Missing input and output files.");
    }
    case 2: {
        // Example: app --help 
        if (cli_is_help(arguments[1]))
            cli_help(cli);
        else
            cli_panic(cli, "Missing output file.");
    }
    default: { 
        cli_panic(cli, "Wrong argument count");
    }
    }

    if (!cli_is_bmp(cli->input_file) || !cli_is_bmp(cli->output_file))
        cli_panic(cli, "Both, input and output files must be Bitmaps (.bmp)");

    return cli;

}


void cli_free(Cli* cli) {
    free(cli);
}