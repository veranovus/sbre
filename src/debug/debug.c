#include "common.h"


#ifndef DEBUG


/* OpenGL Error Check Functions */

void SBRE_clear_opengl_error() {

    while (glGetError() != GL_NO_ERROR) {}
}



void SBRE_check_opengl_error() {

    GLenum error;
    while((error = glGetError()) != GL_NO_ERROR)
        printf("[OpenGL Error][Error : (%u), @%s in line %d]\n", error, __func__, __LINE__);
}



#endif
