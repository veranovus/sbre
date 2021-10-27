#include "common.h"


/* GLFW / Window */

GLFWwindow* _SBRE_main_window;



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

Texture* SBRE_load_texture(const char* filepath) {
	
	uint32_t texture_id;
	int width, height, bpp;
	
	stbi_set_flip_vertically_on_load(1);
	unsigned char* local_buffer = stbi_load(filepath, &width, &height, &bpp, 4);
	
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	stbi_image_free(local_buffer);
	
	Texture* t = (Texture*) malloc(sizeof(Texture));
	*t = (Texture){
		.texture_id = texture_id,
		.color = (Color) { 1.0f, 1.0f, 1.0f, 1.0f },
		.width = width,
		.height = height,
		.bpp = bpp
	};
	
	return t;
}




