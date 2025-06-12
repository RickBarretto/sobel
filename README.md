# Sobel

![Sobels's Banner](./.github/banner.png)

> [!NOTE]
>
> This branch is designed for standalone desktop usage.
> All the same features from the main branch are implemented here,
> but **no FPGA required**.

## At A Glance

| Original                                                    | Processed with Sobel Filter                       |
|-------------------------------------------------------------|---------------------------------------------------|
| ![Original Image from Brat's movie](./.github/original.bmp) | ![Sobel applied output](./.github/post-sobel.bmp) |

**Note:** The original image was first converted to an 8-bit grayscale bitmap using `magick` before applying the filter via this standalone CLI application (not generated from the MPU). For display purposes on GitHub, the images have been converted to PNG format for inclusion in the README.

> [!NOTE]
> **Disclaimer:** We do not claim ownership of the original image, which is a still from the film Brat [Brother] (1997). All rights to the image and film remain with their respective copyright holders.
> 
> **Fair Use Notice:** This image is used under the principles of fair use for non-commercial, educational, or transformative purposes as part of this project. If you are a rights holder and believe this use is inappropriate, please contact us to discuss proper attribution or removal.

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

```sh
cc --std=c99 -I include src/**/*.c -o sobel
```

- **Warning**: depending on your compiler version, `-lm` must be passed at the end. If you're facing issues, pass `-lm` at the end.
- **Warning**: depending on your shell's version, `**` may not work as expected. If you are facing issues, pass: `src/*.c src/*/*.c` as files.

## Tested using
- Gnu C Compiler (GCC) `13.2.0`
- Fish `3.7.1`