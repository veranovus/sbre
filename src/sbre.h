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

#define SBRE_DEFAULT_BATCH_SHADER  3



/* Colors */

#define SBRE_TRANSPARENT	(Color) { 255, 255, 255, 0 }

#define SBRE_WHITE          (Color) { 255, 255, 255, 255 }

#define SBRE_BLACK          (Color) { 0, 0, 0, 255 }

#define SBRE_RED            (Color) { 255, 0, 0, 255 }

#define SBRE_GREEN          (Color) { 0, 255, 0, 255 }

#define SBRE_BLUE           (Color) { 0, 0, 255, 255 }



/* Type Initializers */

#define SBRE_VEC2(x, y)         ((Vec2)  { x, y })

#define SBRE_VEC3(x, y, z)      ((Vec3)  { x, y, z })

#define SBRE_VEC4(x, y, z, w)   ((Vec4)  { x, y, z, w })

#define SBRE_COLOR(r, g, b, a)  ((Color) { r, g, b, a })



/* Texture Filter */

#define SBRE_NEAREST 0x2600

#define SBRE_LINEAR  0x2601



/* Input */

#define SBRE_KEY_UNKNOWN   -1
#define SBRE_KEY_SPACE   32
#define SBRE_KEY_APOSTROPHE   39 
#define SBRE_KEY_COMMA   44 
#define SBRE_KEY_MINUS   45 
#define SBRE_KEY_PERIOD   46 
#define SBRE_KEY_SLASH   47 
#define SBRE_KEY_0   48
#define SBRE_KEY_1   49
#define SBRE_KEY_2   50
#define SBRE_KEY_3   51
#define SBRE_KEY_4   52
#define SBRE_KEY_5   53
#define SBRE_KEY_6   54
#define SBRE_KEY_7   55
#define SBRE_KEY_8   56
#define SBRE_KEY_9   57
#define SBRE_KEY_SEMICOLON   59
#define SBRE_KEY_EQUAL   61 
#define SBRE_KEY_A   65
#define SBRE_KEY_B   66
#define SBRE_KEY_C   67
#define SBRE_KEY_D   68
#define SBRE_KEY_E   69
#define SBRE_KEY_F   70
#define SBRE_KEY_G   71
#define SBRE_KEY_H   72
#define SBRE_KEY_I   73
#define SBRE_KEY_J   74
#define SBRE_KEY_K   75
#define SBRE_KEY_L   76
#define SBRE_KEY_M   77
#define SBRE_KEY_N   78
#define SBRE_KEY_O   79
#define SBRE_KEY_P   80
#define SBRE_KEY_Q   81
#define SBRE_KEY_R   82
#define SBRE_KEY_S   83
#define SBRE_KEY_T   84
#define SBRE_KEY_U   85
#define SBRE_KEY_V   86
#define SBRE_KEY_W   87
#define SBRE_KEY_X   88
#define SBRE_KEY_Y   89
#define SBRE_KEY_Z   90
#define SBRE_KEY_LEFT_BRACKET   91 
#define SBRE_KEY_BACKSLASH   92 
#define SBRE_KEY_RIGHT_BRACKET   93 
#define SBRE_KEY_GRAVE_ACCENT   96 
#define SBRE_KEY_WORLD_1   161 
#define SBRE_KEY_WORLD_2   162 
#define SBRE_KEY_ESCAPE   256
#define SBRE_KEY_ENTER   257
#define SBRE_KEY_TAB   258
#define SBRE_KEY_BACKSPACE   259
#define SBRE_KEY_INSERT   260
#define SBRE_KEY_DELETE   261
#define SBRE_KEY_RIGHT   262
#define SBRE_KEY_LEFT   263
#define SBRE_KEY_DOWN   264
#define SBRE_KEY_UP   265
#define SBRE_KEY_PAGE_UP   266
#define SBRE_KEY_PAGE_DOWN   267
#define SBRE_KEY_HOME   268
#define SBRE_KEY_END   269
#define SBRE_KEY_CAPS_LOCK   280
#define SBRE_KEY_SCROLL_LOCK   281
#define SBRE_KEY_NUM_LOCK   282
#define SBRE_KEY_PRINT_SCREEN   283
#define SBRE_KEY_PAUSE   284
#define SBRE_KEY_F1   290
#define SBRE_KEY_F2   291
#define SBRE_KEY_F3   292
#define SBRE_KEY_F4   293
#define SBRE_KEY_F5   294
#define SBRE_KEY_F6   295
#define SBRE_KEY_F7   296
#define SBRE_KEY_F8   297
#define SBRE_KEY_F9   298
#define SBRE_KEY_F10   299
#define SBRE_KEY_F11   300
#define SBRE_KEY_F12   301
#define SBRE_KEY_F13   302
#define SBRE_KEY_F14   303
#define SBRE_KEY_F15   304
#define SBRE_KEY_F16   305
#define SBRE_KEY_F17   306
#define SBRE_KEY_F18   307
#define SBRE_KEY_F19   308
#define SBRE_KEY_F20   309
#define SBRE_KEY_F21   310
#define SBRE_KEY_F22   311
#define SBRE_KEY_F23   312
#define SBRE_KEY_F24   313
#define SBRE_KEY_F25   314
#define SBRE_KEY_KP_0   320
#define SBRE_KEY_KP_1   321
#define SBRE_KEY_KP_2   322
#define SBRE_KEY_KP_3   323
#define SBRE_KEY_KP_4   324
#define SBRE_KEY_KP_5   325
#define SBRE_KEY_KP_6   326
#define SBRE_KEY_KP_7   327
#define SBRE_KEY_KP_8   328
#define SBRE_KEY_KP_9   329
#define SBRE_KEY_KP_DECIMAL   330
#define SBRE_KEY_KP_DIVIDE   331
#define SBRE_KEY_KP_MULTIPLY   332
#define SBRE_KEY_KP_SUBTRACT   333
#define SBRE_KEY_KP_ADD   334
#define SBRE_KEY_KP_ENTER   335
#define SBRE_KEY_KP_EQUAL   336
#define SBRE_KEY_LEFT_SHIFT   340
#define SBRE_KEY_LEFT_CONTROL   341
#define SBRE_KEY_LEFT_ALT   342
#define SBRE_KEY_LEFT_SUPER   343
#define SBRE_KEY_RIGHT_SHIFT   344
#define SBRE_KEY_RIGHT_CONTROL   345
#define SBRE_KEY_RIGHT_ALT   346
#define SBRE_KEY_RIGHT_SUPER   347
#define SBRE_KEY_MENU   348
#define SBRE_KEY_LAST   SBRE_KEY_MENU



/* Typdefs */

typedef uint32_t SBRE_Shader;



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



typedef struct Rectangle {
	Vec2 position;
	float width, height;
} Rectangle;



typedef struct Texture {
	uint32_t texture_id;
	Vec4 color;
	int initial_width, initial_height;
	int width, height, bpp;
} Texture;



typedef struct SBRE_Character {
    char character;
    Vec2 render_offset;
    Vec2 size;
    Vec2 bearing;
    uint32_t advance;
} SBRE_Character;



typedef struct Font {
    uint32_t font_size;
	Vec2 biggest_char;
    Texture* font_atlas;
    SBRE_Character* _characters;
} Font;



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



void SBRE_free_texture(Texture* texture);



/* Use this to change a certain part of a texture to another texture. Original texture size (size of the image in pixels) 
 * is used here so, you cant load a texture bigger than the target. */
void SBRE_load_texture_to_texture(Texture* texture, const char* filepath, int32_t offset_x, int32_t offset_y);



/* Font */

Font* SBRE_create_font(const char* font_path, uint32_t font_size, uint32_t filter);



/* Use this function to free the memory allocated for the font */
void SBRE_free_font(Font* font);



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



/* Individual Render Functions (Not batched) */

/* Draws a colored quad with the given parameters */
void SBRE_draw_quad(Vec2 pos, float width, float height, Color color);



void SBRE_draw_quad_ext(Vec2 pos, float width, float height, float rotation, Color color);



/* Draws a quad outlien with given parameters. This function clears the STENCIL_BUFFER_BIT be aware of that. */
void SBRE_draw_quad_outline(Vec2 pos, float width, float height, float border, Color quad_color, Color border_color);



void SBRE_draw_quad_outline_ext(Vec2 pos, float width, float height, float border, float rotation, Color quad_color, Color border_color);



void SBRE_draw_circle(Vec2 pos, float radius, Color color);



/* Draws 2 circles, one for the fill and one for the outline. Doesn't clears the STENCIL_BUFFER_BIT. */
void SBRE_draw_circle_outline(Vec2 pos, float radius, float border, Color fill_color, Color outline_color);



void SBRE_draw_texture(Vec2 pos, Texture* texture, Rectangle* texture_rect);



void SBRE_draw_texture_ext(Vec2 pos, Texture* texture, Rectangle* texture_rect, float rotation);



void SBRE_draw_text(Vec2 pos, const char* text, Font* font, Color color);

