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

**For debug**

```sh
cc --std=c99 -I include src/*.c -o sobel
```


**For production**

```sh
cc --std=c99 -I include src/*.c src/mpu.s -D IN_PROD -o sobel
```

- You must compile `mpu.s`
- You must pass `IN_PROD`