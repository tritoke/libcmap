# Colourmaps

This is a very simple format for storing colourmaps for various graphics projects I've made.
Originally stemming from my mandelbrot set renderer.

## Building

Just run `make` and you should be good to go.
It will produce `libcmap.a` in the current directory, add that path to your compile command and add `-lcmap` to link.

For example I have the following as part of a make rule which uses libcmap: `LIBCMAP = -L${CMAPINC} -lcmap`.

## Examples
This goes through all the colourmaps, using [interpol](https://github.com/tritoke/interpol) to generate frames in between discrete renders.
https://www.youtube.com/watch?v=YegqR2oB9qQ
