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
    printf("    $ %s %s\n", cli->app, code);
}

void cli_help(Cli* cli) {
    printf("Usage: %s %s\n", cli->app, cli->usage);
    printf("%s\n", cli->description);

    cli_print_section("Options");
    cli_option("--help | -h       display this.");
    cli_option("--sobel | -s      apply Sobel filter.");
    cli_option("--ext-sobel | x   apply Extended Sobel filter.");
    cli_option("--prewitt | -p    apply Prewitt filter.");
    cli_option("--roberts | -r    apply Roberts filter.");
    cli_option("--laplacian | -l  apply Laplacian filter.");

    cli_print_section("Example");
    cli_example(cli, "input.bmp output.bmp");
    cli_example(cli, "--prewitt input.bmp output.bmp");
    cli_example(cli, "-p input.bmp output.bmp");
    cli_example(cli, "data/input.bmp output.bmp -x");

    puts("");

    cli_free(cli);
    exit(EXIT_SUCCESS);
}


void cli_panic(Cli* cli, cstring_t error) {
    fprintf(stderr,"%s: %s\n", cli->app, error);
    fprintf(stderr, "Try '%s --help' for more information.\n", cli->app);

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

bool cli_is_flag(cstring_t argument) {
    return argument[0] == '-';
}

bool cli_is_sobel(cstring_t flag) {
    return cli_str_equals(flag, "--sobel")
        || cli_str_equals(flag, "-s");
}

bool cli_is_extended_sobel(cstring_t flag) {
    return cli_str_equals(flag, "--ext-sobel")
        || cli_str_equals(flag, "-x");
} 

bool cli_is_prewitt(cstring_t flag) {
    return cli_str_equals(flag, "--prewitt")
        || cli_str_equals(flag, "-p");
}

bool cli_is_roberts(cstring_t flag) {
    return cli_str_equals(flag, "--roberts")
        || cli_str_equals(flag, "-r");
}

bool cli_is_laplacian(cstring_t flag) {
    return cli_str_equals(flag, "--laplacian")
        || cli_str_equals(flag, "-l");
}

Filter cli_filter_from(cstring_t flag) {
    #define map(_name, _enum) if (cli_is_ ## _name (flag)) return _enum
    map(sobel, Sobel);
    map(extended_sobel, ExtendedSobel);
    map(prewitt, Prewitt);
    map(roberts, Roberts);
    map(laplacian, Laplacian);
    return Sobel;
}

// =~=~=~=~=~=~=~=~ Public ~=~=~=~=~=~=~=~= 


Cli *cli_from(cstring_t app, cstring_t description, cstring_t arguments[], size_t count) {
    cstring_t usage = "<bitmap input> <bitmap output>";
    Cli* cli = cli_new(app, description, usage);

    cli->input_file = NULL;
    cli->output_file = NULL;

    bool input_found = false;
    bool output_found = false;

    if (count == 2 && cli_is_help(arguments[1])) {
        cli_help(cli);
    }

    if (count < 3) {
        cli_panic(cli, "Missing output file.");
    }

    for (int i = 1; i < count; i++) {
        cstring_t current = arguments[i];
        if (cli_is_flag(current)) {
            if (cli_is_help(current)) cli_help(cli);
            cli->filter = cli_filter_from(current);            
        } else {
            unless (input_found) {
                cli->input_file = current;
                input_found = true;
                continue;
            }
            unless (output_found) {
                cli->output_file = current;
                output_found = true;
                continue;
            }
        }
    }

    unless (cli->input_file) {
        cli_panic(cli, "Missing input file.");
    }
    
    unless (cli->output_file) {
        cli_panic(cli, "Missing output file.");
    }

    if (!cli_is_bmp(cli->input_file) || !cli_is_bmp(cli->output_file))
        cli_panic(cli, "Both, input and output files must be Bitmaps (.bmp)");

    return cli;

}


void cli_free(Cli* cli) {
    free(cli);
}