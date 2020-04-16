# 3D Game Programming from Scratch

## Dependencies

This installs GCC and the C dependencies along with SDL2.

```text
$ sudo apt install build-essential
$ sudo apt install libsdl2-dev
```

## Structure

His preferred structure:

```text
3drenderer
├── Makefile
└── src
    ├── main.c
    ├── mesh.c
    └── mesh.h
```


```text
$ gcc main.c -o renderer
$ ./renderer
```

It can get more complex like this:

```text
$ gcc -Wall -std=c99 src/*.c -o renderer
$ ./renderer
```

