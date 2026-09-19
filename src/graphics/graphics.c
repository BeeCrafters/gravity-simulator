#include <stdio.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#ifndef __GRAPHICS_C__
#define __GRAPHICS_C__

void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
	glViewport(0, 0, w, h);
}

void on_input(GLFWwindow* window) {
}

#endif
