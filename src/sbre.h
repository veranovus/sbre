#pragma once


/* C Standart Library Dependencies */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <memory.h>
#include <stdbool.h>



/* Macros*/

/* Renderer */

/* SBRE's default shaders id, use this to activate default shader */
#define SBRE_DEFAULT_SHADER  1



/* Colors */

#define SBRE_TRANSPARENT 	(Color) { 255, 255, 255, 255 }

#define SBRE_WHITE 			(Color) { 255, 255, 255, 255 }

#define SBRE_BLACK 			(Color) { 0, 0, 0, 255 }

#define SBRE_RED 			(Color) { 255, 0, 0, 255 }

#define SBRE_GREEN 			(Color) { 0, 255, 0, 255 }

#define SBRE_BLUE 			(Color) { 0, 0, 255, 255 }



/* Type Initializers */

#define SBRE_VEC2(x, y) 		((Vec2)  { x, y })

#define SBRE_VEC3(x, y, z)		((Vec3)  { x, y, z })

#define SBRE_VEC4(x, y, z, w) 	((Vec4)  { x, y, z, w })

#define SBRE_COLOR(r, g, b, a) 	((Color) { r, g, b, a })




/* Typdefs */



/* Structs */

typedef struct Vec2 {
	float x, y;
} Vec2;



typedef struct Vec3 {
	float x, y, z;
} Vec3;



typedef struct Vec4 {
	float x, y, z, w;
} Vec4;



typedef struct Color {
	float r, g, b, a;
} Color;



typedef struct Mat4 {
	float r1[4];
	float r2[4];
	float r3[4];
	float r4[4];
} Mat4;



typedef struct Texture {
	uint32_t texture_id;
	Vec4 color;
	int width, height, bpp;
} Texture;



/* Core */

/* Initializez GLFW, GLEW and all other necessary components of the engine,
 * creates a GLFW window and returns a reference to GLFW window. */
bool SBRE_init(int SCREEN_WIDTH, int SCREEN_HEIGHT, const char* TITLE, int VSync);



/* Terminates the engine, does not terminates the external dependencies */
void SBRE_terminate(void);



/* Returns true if window should close */
bool SBRE_window_should_close(void);



/* Sets the clear color to the given color, color values should be between 0 and 1
 * rather than 0 to 255, this will change in the future. */
void SBRE_set_clear_color(Color clear_color);



/* Clears the COLOR_BUFFER_BIT and the STENCIL_BUFFFER_BIT each time its called. 
 * Simply clears the display */
void SBRE_clear(void);



void SBRE_display(void);



void SBRE_poll_events(void);



/* Texture */

Texture* SBRE_load_texture(const char* filepath);



/* Shader */

/* Sets the shader to be used when rendering */
void SBRE_use_shader(uint32_t shader);



/* Creates a shader program from given shader source files and returns a handle to the compiled shader. */
uint32_t SBRE_create_shader(const char* vertexShaderSource, const char* fragmentShaderSource);



/* Creates a shader program from given string constants, compiling shaders from string constants are 
 * not advised, since it will be harder for you to make changes and debug. */
uint32_t _SBRE_create_shader_from_string(const char* vertex_source, const char* fragment_source);



/* Compiles a shader from given shader soruce returns 0 if shader compilation fails 
 * Don't call this function unless you're going to link returned shader yourself, use 
 * SBRE_create_shader instead. */
uint32_t _SBRE_compile_shader(uint32_t type, const char* shader_source);



/* Parse the shader source code from a give file, psth has to be relative with
 * the executable, yet this might depend on your compiler or IDE. Calling this
 * function implicitly and not freeing the returned memory will cause in memory leak */
const char* _SBRE_parse_shader(const char* shader_source);



/* Shader Uniforms */

bool SBRE_set_uniform_1f(uint32_t shader, const char* name, float value);



bool SBRE_set_uniform_2f(uint32_t shader, const char* name, Vec2 floats);



bool SBRE_set_uniform_3f(uint32_t shader, const char* name, Vec3 floats);



bool SBRE_set_uniform_4f(uint32_t shader, const char* name, Vec4 floats);



bool SBRE_set_uniform_mat4f(uint32_t shader, const char* name, Mat4 matrix);



/* MVP Matrix */

/* Sets the default projection matrix of the renderer, this is automatically done when the window is created. */
void _SBRE_set_projection_marix(Mat4 proj);



/* Don't call this function, just pass in a Camera to the render functions when calling them. */
void _SBRE_set_view_matrix(Mat4 view);



/* Individual Render Functions */

/* Draws a colored quad with the given parameters */
void SBRE_draw_quad(Vec2 pos, float width, float height, Color color);



/* Draws a quad outlien with given parameters. This function clears the STENCIL_BUFFER_BIT be aware of that. */
void SBRE_draw_quad_outline(Vec2 pos, float width, float height, float border, Color quad_color, Color border_color);



void SBRE_draw_texture(Vec2 pos, Texture* texture);




