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

## Preparing images

Make sure you have [Image Magick](https://imagemagick.org/) installed in your system.
With this tool, run:

```sh
magick data/input.png -colorspace Gray -depth 8 -type Grayscale BMP3:input.bmp
```


## Compilation

**For debug**

```sh
cc --std=c99 -lm -I include src/*.c -o sobel
```


**For production**

```sh
cc --std=c99 -lm -I include src/*.c src/mpu.s -D IN_PROD -o sobel
```

- You must compile `mpu.s`
- You must pass `IN_PROD`