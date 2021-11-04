#pragma once
/*
 * Common includes and dependencies of the SBRE library, this file is included in almost 
 * every SBRE source file, and stores engine spesific macros, typedefs and includes.  */


/* Core Includes */
#include <GL/glew.h>
#include <GLFW/glfw3.h>


/* External Includes */
#include <stb_image/stb_image.h>
#include <cglm/call.h>


/* C Standart Library  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <memory.h>
#include <stdbool.h>



/* Macros */

/* Renderer */

#define SBRE_DEFAULT_SHADER  1

#define NORMALIZE_RGBA(r, g, b, a) ((Color) { r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f })



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



/* Texture Filter */

#define SBRE_NEAREST 0x2600

#define SBRE_LINEAR 0x2601



/* Window */

extern uint32_t _SBRE_WINDOW_HEIGHT;
extern uint32_t _SBRE_WINDOW_HEIGHT;



/* Renderer */

extern uint32_t _SBRE_active_shader;



/* Default Shaders */

extern const char* _SBRE_vertex_shader_source;



extern const char* _SBRE_fragment_shader_source;



extern const char* _SBRE_circle_fragment_shader_source;



/* SBRE types */

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
	Color color;
	int width, height, bpp;
} Texture;



/* Core */

bool SBRE_init(int SCREEN_WIDTH, int SCREEN_HEIGHT, const char* TITLE, int VSync);



void SBRE_terminate(void);



bool SBRE_window_should_close(void);



void SBRE_set_clear_color(Color clear_color);



void SBRE_clear(void);



void SBRE_display(void);



void SBRE_poll_events(void);



/* Delta Time */

void SBRE_calculate_delta_time(void);



double SBRE_delta_time(void);



/* FPS */

void SBRE_calculate_fps(void);



uint32_t SBRE_fps(void);



double SBRE_frame_time(void);



/* Input */ 

bool SBRE_get_key_press(int key_code);



bool SBRE_get_key_release(int key_code);



/* Texture */

Texture* SBRE_load_texture(const char* filepath, uint32_t filter);



/* Shader */

void SBRE_use_shader(uint32_t shader);



uint32_t SBRE_create_shader(const char* vertexShaderSource, const char* fragmentShaderSource);



uint32_t _SBRE_create_shader_from_string(const char* vertex_source, const char* fragment_source);



uint32_t _SBRE_compile_shader(uint32_t type, const char* shader_source);



const char* _SBRE_parse_shader(const char* shader_source);



/* Shader Uniforms */

bool SBRE_set_uniform_1f(uint32_t shader, const char* name, float value);



bool SBRE_set_uniform_2f(uint32_t shader, const char* name, Vec2 floats);



bool SBRE_set_uniform_3f(uint32_t shader, const char* name, Vec3 floats);



bool SBRE_set_uniform_4f(uint32_t shader, const char* name, Vec4 floats);



bool SBRE_set_uniform_mat4f(uint32_t shader, const char* name, Mat4 matrix);



/* MVP Matrix */

void _SBRE_set_projection_marix(Mat4 proj);



void _SBRE_set_view_matrix(Mat4 view);



/* Renderer */

void _SBRE_init_render_systems(void);



void _SBRE_terminate_render_systems(void);



/* Individual Render Functions */

void SBRE_draw_quad(Vec2 pos, float width, float height, Color color);



void SBRE_draw_quad_outline(Vec2 pos, float width, float height, float border, Color quad_color, Color border_color);



void SBRE_draw_texture(Vec2 pos, Texture* texture);



void SBRE_draw_circle(Vec2 pos, float radius, Color color);



#ifndef NDEBUG
/* Debugging */

void SBRE_clear_opengl_error();



void SBRE_check_opengl_error();



#endif




