# Sobel

> [!INFO] Branches: main & indie
> 
> The main branch contains the final project submission 
> for the TEC499 course, designed specifically for the 
> Cyclone V SoC HPS+FPGA board. It is hardware-dependent 
> and requires the FPGA platform to run.
> 
> On the other hand, the indie branch is a standalone 
> version built for desktop use. It’s a fully functional 
> C-based CLI application that allows you to explore and 
> experiment with the algorithms—no FPGA required!
> 
> Don’t have a Cyclone V board? No problem—check out the 
> indie branch and start having fun with the algorithms. 
> :wink:

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
cc --std=c99 -I include src/*.c src/**/*.{c,s} -D IN_PROD -o sobel -lm
```

- You must compile `mpu.s`
- You must pass `IN_PROD`