# Sobel


![Sobel's Banner](./.github/banner.png)


> [!IMPORTANT] 
> 
> ## Branches: main & standalone
> 
> The `main` branch contains the final project submission 
> for the ***TEC499 - Digital Systems** course*, designed specifically for the 
> *Cyclone V SoC HPS+FPGA* board. **It is hardware-dependent 
> and requires the *FPGA* platform to run**.
> 
> On the other hand, the `standalone` branch is a standalone 
> version *built for desktop use*. It’s a fully functional 
> C-based CLI application that allows you to explore and 
> experiment with the algorithms — **No FPGA required**!
> 
> Don’t have a *Cyclone V* board? No problem — check out the 
> [`standalone` branch](https://github.com/RickBarretto/sobel/tree/standalone) and start having fun with the algorithms. 😉


## Usage

```sh
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
cc --std=c99 -I include src/*.c src/*/*.{c,s} -o sobel -lm
```

- You must compile `mpu.s`

---

> [!IMPORTANT]
> This repository was created for the _**TEC499 – Digital Systems** course_.
> As such, the remainder of this README will be written in Portuguese.
> A future continuation or supplementary documentation may be provided in English.

---

## Relatório Técnico

> [!NOTE]
> Apesar do histórico de commits, 
> é importante destacar que ele não reflete com precisão o empenho ou 
> a contribuição individual de cada membro da equipe.
> 
> Como todo o desenvolvimento foi realizado em ambiente de laboratório, 
> e o acesso seguro às ferramentas Git e GitHub de forma segura foi limitado, 
> o registro de commits não deve ser utilizado como parâmetro de avaliação.
> 
> Também vale ressaltar que, pelas mesmas limitações, 
> nem todas as boas práticas de versionamento 
> e desenvolvimento foram seguidas à risca ao longo do projeto. 
> Por isso, é possível encontrar trechos de código morto utilizados para depuração (debug) e 
> até mesmo commits contendo patches completos.
