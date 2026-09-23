# gravity-simulator

This project is still in development. We have yet to make plans or a mission declaration. I am writing this without personally knowing what the project aims to accomplish. We are currently having fun. As a great mind once said:

> Doesn't make sense not to live for fun;
> Your brain gets **smart**, but your head gets **dumb**.

*Smash Mouth*, ***All Star***, from album ***Astro Lounge*** (1999)

## Compilation

For your first time running `make`, be sure to have an internet connection. The Makefile includes the line:

`	curl https://raw.githubusercontent.com/jkuhlmann/cgltf/refs/heads/master/cgltf.h`

### - Dependencies

- glfw

### - The GLFW situation

We have left the choice of a local GLFW installation to the user. This is because some team members run different OSs and don't have proper glfw packages. Obviously `make` won't run outside of a posix environment but this can be fixed with `MSYS2` or `mingw-w64` for Windows users.

Upon running make for the first time, you will be asked to declare your installation type. The rest is on you.

If you have selected the local installation type, there will be instructions for you to put your installed glfw libraries/headers in the correct directories.

If you have selected system-wide, pkg-config will be run to find the correct C flags.

### - Actually Running `make`

- `make`

This will build the application.

- `make debug`

This will build the application with debug features. This target adds the `-g` flag to `gcc` and also defines the `__DEBUG__` macro.

---

A huge thank you to:
- Contributors of [cgltf](https://github.com/jkuhlmann/cgltf/tree/master)
- Contributors of [glad](https://github.com/Dav1dde/glad)
- Contributors of [glfw](https://github.com/glfw/glfw)
