# 3drenderer

## Dependencies

This installs GCC and the C dependencies along with SDL2.

```text
$ sudo apt install build-essential
$ sudo apt install libsdl2-dev
```

## Structure

Sample structure:

```text
3drenderer
├── Makefile
└── src
    ├── main.c
    ├── mesh.c
    └── mesh.h
```

## Compiling

The most simple:

```text
$ gcc main.c -o renderer
$ ./renderer
```

See the `Makefile`.
