#include "common.h"



/* GLFW / Window */

GLFWwindow* _SBRE_main_window;



/* Delta Time Varaibles */

static const double _SBRE_delta_time_high = 0.1;
static double _SBRE_last_frame_time;
static double _SBRE_current_frame_time;
double _SBRE_delta_time;



/* FPS Variables */

static double _SBRE_previous_time;
static double _SBRE_current_time;
static double _SBRE_time_diff;
static uint32_t _SBRE_fps_counter;
static uint32_t _SBRE_FPS;
static double _SBRE_frame_time;



/* Delta Time */

void SBRE_calculate_delta_time(void) {

    _SBRE_current_frame_time = glfwGetTime();

    _SBRE_delta_time = _SBRE_current_frame_time - _SBRE_last_frame_time;
    if (_SBRE_delta_time > _SBRE_delta_time_high)
        _SBRE_delta_time = _SBRE_delta_time_high;

    _SBRE_last_frame_time = _SBRE_current_frame_time;
}



double SBRE_delta_time(void) {

	return _SBRE_delta_time;
}



/* FPS */

void SBRE_calculate_fps(void) {

	_SBRE_current_time = glfwGetTime();
    _SBRE_time_diff = _SBRE_current_time - _SBRE_previous_time;
    _SBRE_fps_counter++;

    if (_SBRE_time_diff >= 1 / 30.0) {
		_SBRE_FPS = (1.0 / _SBRE_time_diff) * _SBRE_fps_counter;
        _SBRE_frame_time = (_SBRE_time_diff / _SBRE_fps_counter) * 1000;
        _SBRE_previous_time = _SBRE_current_time;
		_SBRE_fps_counter = 0;
    }
}



uint32_t SBRE_fps(void) {
	
	return _SBRE_FPS;
}



double SBRE_frame_time(void) {

	return _SBRE_frame_time;
}



/* Input */

bool SBRE_get_key_press(int key_code) {

	return glfwGetKey(_SBRE_main_window, key_code) == GLFW_PRESS;
}



bool SBRE_get_key_release(int key_code) {

	return glfwGetKey(_SBRE_main_window, key_code) == GLFW_RELEASE;
}




/* Core Initialization */

bool SBRE_init(int SCREEN_WIDTH, int SCREEN_HEIGHT, const char* TITLE, int vsync) {
	
	// Initialize GLFW

    if(!glfwInit()) {
		printf("[GLFW Error][GLFW could not be initialized!]\n");
        return false;
    }

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


	// Create window

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE, NULL, NULL);
    if (!window) {
		printf("[GLFW Error][Window could now be initialized!]\n");
        return false;
    }


	// Make the window current context

    glfwMakeContextCurrent(window);


    // Enable-Disable V-Sync

    glfwSwapInterval(vsync);


    // Initialize GLEW

    if (glewInit() != GLEW_OK) {
		printf("[GLEW Error][GLEW could not be initialized!]\n");
        glfwTerminate();
        return false;
    }


	// Set the main_window

	_SBRE_main_window = window;


	// Initialize Rendering Systems

	_SBRE_init_render_systems();


	// Set the default Projection Matrix

	mat4 proj;
	glm_mat4_identity(proj);
	glm_ortho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f, proj);	
	
	Mat4 mvp = {
		.r1 = { proj[0][0], proj[0][1], proj[0][2], proj[0][3] },
		.r2 = { proj[1][0], proj[1][1], proj[1][2], proj[1][3] },
		.r3 = { proj[2][0], proj[2][1], proj[2][2], proj[2][3] },
		.r4 = { proj[3][0], proj[3][1], proj[3][2], proj[3][3] },
	};

	_SBRE_set_projection_marix(mvp);

	return true;
}



void SBRE_terminate(void) {
	
	_SBRE_terminate_render_systems();

	glfwTerminate();
}



/* Core */

bool SBRE_window_should_close(void) {
	
	return glfwWindowShouldClose(_SBRE_main_window);
}



void SBRE_set_clear_color(Color clear_color) {
	
	glClearColor(clear_color.r / 255.0f, clear_color.g / 255.0f, clear_color.b / 255.0f, clear_color.a / 255.0f);
}



void SBRE_clear(void) {
	
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}



void SBRE_display(void) {
	
	glfwSwapBuffers(_SBRE_main_window);
}



void SBRE_poll_events(void) {
	
	glfwPollEvents();
}



/* Texture */

Texture* SBRE_load_texture(const char* filepath, uint32_t filter) {
	
	FILE* fp = NULL;
	fp = fopen(filepath, "r");

	if (!(fp)) {
		printf("[SBRE Error][Image file \"%s\" could not be opened.]\n", filepath);
		fclose(fp);
		return NULL;
	}
	
	fclose(fp);

	uint32_t texture_id;
	int width, height, bpp;
	
	stbi_set_flip_vertically_on_load(1);
	unsigned char* local_buffer = stbi_load(filepath, &width, &height, &bpp, 4);
	
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	stbi_image_free(local_buffer);
	
	Texture* t = (Texture*) malloc(sizeof(Texture));
	*t = (Texture){
		.texture_id = texture_id,
		.color = SBRE_COLOR(255, 255, 255, 255),
		.width = width,
		.height = height,
		.bpp = bpp
	};
	
	return t;
}


// FIXME : FIX THIS

/*
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include <cimgui/cimgui.h>
#include <cimgui/imgui_impl_glfw.h>
#include <cimgui/imgui_impl_opengl3.h>

static void error_callback(int e, const char *d)
{printf("Error %d: %s\n", e, d);}


struct ImGuiContext* ctx;
struct ImGuiIO* io;
    
void gui_init() {
    // IMGUI_CHECKVERSION();
    ctx = igCreateContext(NULL);
    io  = igGetIO();

    const char* glsl_version = "#version 330 core";
    ImGui_ImplGlfw_InitForOpenGL(_SBRE_main_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    igStyleColorsDark(NULL);
}

void gui_terminate() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    igDestroyContext(ctx);
}

void gui_render() {
    igRender();
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());
}

void gui_update() {
	
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    igNewFrame();

    igBegin("Test", NULL, 0);
    igText("Test");
    igButton("Test",(struct ImVec2){0,0});
    igEnd();

    // // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. 
    // // Here we just want to make the demo initial state a bit more friendly!
    // igSetNextWindowPos((struct ImVec2){0,0}, ImGuiCond_FirstUseEver,(struct ImVec2){0,0} ); 
    igShowDemoWindow(NULL);
}*/