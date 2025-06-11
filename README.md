# Sobel

![Sobels's Banner](./.github/banner.png)

> [!NOTE]
>
> This branch is designed for standalone desktop usage.
> All the same features from the main branch are implemented here,
> but **no FPGA required**.

## Usage

```
Usage: sobel <bitmap input> <bitmap output>
Apply border filters to bitmap images.

Options:
    --help | -h       display this.
    --sobel | -s      apply Sobel filter.
    --ext-sobel | x   apply Extended Sobel filter.
    --prewitt | -p    apply Prewitt filter.
    --roberts | -r    apply Roberts filter.
    --laplacian | -l  apply Laplacian filter.

Example:
    $ sobel input.bmp output.bmp
    $ sobel --prewitt input.bmp output.bmp
    $ sobel -p input.bmp output.bmp
    $ sobel data/input.bmp output.bmp -x

```

## Preparing images

Make sure you have [Image Magick](https://imagemagick.org/) installed in your system.
With this tool, run:

```sh
magick data/input.png -colorspace Gray -depth 8 -type Grayscale BMP3:input.bmp
```

## Compilation

**For debug**

```sh
cc --std=c99 -I include src/**/*.c -o sobel
```

- **Warning**: depending on your compiler version, `-lm` must be passed at the end. If you're facing issues, pass `-lm` at the end.
- **Warning**: depending on your shell's version, `**` may not work as expected. If you are facing issues, pass: `src/*.c src/*/*.c` as files.

## Tested using
- Gnu C Compiler (GCC) `13.2.0`
- Fish `3.7.1`