# Sobel

## Usage

```sh
Usage: sobel <bitmap input> <bitmap output>

Options:
    --help | -h       displays this.

Example:
    $ sobel input.bmp output.bmp
```

## Compilation

```sh
cc --std=c99 -I include src/*.c -o sobel
```