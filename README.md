# Sobel

## Usage

```
Usage: sobel <bitmap input> <bitmap output>
Apply border filters to bitmap images.

Options:
    --help | -h       displays this.

Example:
    $ sobel input.bmp output.bmp

```

## Compilation

```sh
cc --std=c99 -I include src/*.c -o sobel
```