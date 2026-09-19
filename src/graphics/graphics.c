/*
 * Main graphics/engine/window code.
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <math/vec.h>
#include <debug.h>
#include <list.h>

#include <stdlib.h>
#include <string.h>

#ifndef __GRAPHICS_C__
#define __GRAPHICS_C__

// Structs/Enums

typedef enum gfx_struct_type_e:uint64_t {
	GFX_CAMERA,
	GFX_MODEL,
	GFX_OBJECT
} gfx_struct_type;

typedef struct {
	gfx_struct_type type;
	uint64_t id;

	list_node list;
} gfx_camera;

/*
 * This struct is internal, only a handle will be given to the user.
 *
 * Maybe we'll just shill the pointer to this structure as the handle
 * to save processing time, since the handle is a uint64_t.
 */
typedef struct {
	gfx_struct_type type;
	uint64_t handle;
	uint64_t tags;

	void* vertex_array;
	uint64_t vertex_count;
	uint64_t vertex_size;

	void* index_array;
	uint64_t index_count;
	uint64_t index_size;

	list_node list;
} gfx_model;

/*
 * Objects are able to have multiple models attached to them, with multiple
 * cameras too.
 *
 * I don't really know why someone would want multiple cameras
 * attached to a single body, but maybe they would.
 */
typedef struct {
	uint64_t tags;
} gfx_object;

typedef struct {
	GLFWwindow* window;
	char* vertex_shader_path;
	char* geometry_shader_path;
	char* fragment_shader_path;
	ivec2_t window_dimensions;
	void (*key_callback)(int key, int scancode, int action, int mods);
	void (*cursor_callback)(double x, double y);
	void (*button_callback)(int key, int action, int mods);
	void (*scroll_callback)(double xoffset, double yoffset);
} gfx_state;



// Variables

static gfx_state graphics_state = {
	.window = NULL,
	.vertex_shader_path = NULL,
	.geometry_shader_path = NULL,
	.fragment_shader_path = NULL,
	.window_dimensions = ivec2(800, 600),
};



// Static Fuctions

static void gfx_on_framebuffer_resize(GLFWwindow* window, int w, int h) {
	graphics_state.window_dimensions.w = w;
	graphics_state.window_dimensions.h = h;
	glViewport(0, 0, w, h);
}

static void gfx_on_key(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (!(graphics_state.key_callback)) return;
	graphics_state.key_callback(key, scancode, action, mods);
}

static void gfx_on_cursor(GLFWwindow* window, double x, double y) {
	if (!(graphics_state.cursor_callback)) return;
	graphics_state.cursor_callback(x, y);
}

static void gfx_on_button(GLFWwindow* window, int button, int action, int mods) {
	if (!(graphics_state.button_callback)) return;
	graphics_state.button_callback(button, action, mods);
}

static void gfx_on_scroll(GLFWwindow* window, double xoffset, double yoffset) {
	if (!(graphics_state.scroll_callback)) return;
	graphics_state.scroll_callback(xoffset, yoffset);
}

/*
 * Just loads the file at path into memory. All memory is allocated with
 * malloc and can be freed anytime.
 *
 * Returns NULL on file io error, return pointer should be checked before
 * accessing.
 */
static char* gfx_load_shader(const char* path) {
	FILE* file = fopen(path, "r");
	debug_return(file, NULL);

	fseek(file, 0, SEEK_END);
	size_t filesize = ftell(file);

	char* source = malloc(filesize);
	debug_return(source, NULL);

	fseek(file, 0, SEEK_SET);
	size_t read_size = fread(source, 1, filesize, file);

	if (filesize != read_size) {
		debug_print("In gfx_load_shader, filesize does not equal read_size\n");
		free(source);
		return NULL;
	}

	return source;
}

//TODO
/*
 * If the return value is nonzero, there was an error.
 */
static int gfx_rebuild_program() {
	debug_test((graphics_state.vertex_shader_path == NULL || graphics_state.fragment_shader_path == NULL));
	if (graphics_state.vertex_shader_path == NULL || graphics_state.fragment_shader_path == NULL)
		return 1;

//	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(vertexShader, 1, )
}



// Global Functions (also included in the .h file)

// Set callback functions
void gfx_set_key_callback(void (*callback)(int key, int scancode, int action, int mods)) {
	graphics_state.key_callback = callback;
}

// Set callback functions
void gfx_set_cursor_callback(void (*callback)(double x, double y)) {
	graphics_state.cursor_callback = callback;
}

// Set callback functions
void gfx_set_button_callback(void (*callback)(int key, int action, int mods)) {
	graphics_state.button_callback = callback;
}

// Set callback functions
void gfx_set_scroll_callback(void (*callback)(double xoffset, double yoffset)) {
	graphics_state.scroll_callback = callback;
}

/*
 * This function is to be called once per program execution.
 */
int gfx_init(const char* wname, ivec2_t size) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(size.width, size.height, wname, NULL, NULL);
	debug_return(window, -1);

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, gfx_on_framebuffer_resize);

	debug_return(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), -1);

	graphics_state.window = window;

	return 0;
}

/*
 * The path can be freed or modified anytime after the function returns.
 *
 * If the return value is nonzero, fopen(path, "r") failed and the shader
 * path was not set.
 */
int gfx_set_shader_path(GLenum type, const char* path) {
	FILE* file = fopen(path, "r");
	fclose(file);
	debug_return(file, 1);

	char* variable_path = strdup(path);

	switch (type) {
		case GL_VERTEX_SHADER:
			graphics_state.vertex_shader_path = variable_path;
			break;

		case GL_GEOMETRY_SHADER:
			graphics_state.geometry_shader_path = variable_path;
			break;

		case GL_FRAGMENT_SHADER:
			graphics_state.fragment_shader_path = variable_path;
			break;
	}

	return 0;
}

//TODO
int gfx_destroy() {
}

#endif
