#include "sbre.h"


#include <cglm/call.h>


#include <stdio.h>
#include <stdlib.h>


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TITLE "C OpenGL"
#define VSync 0



int main(void) {
	

	if (!SBRE_init(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE, VSync))
		return -1;


	while(!SBRE_window_should_close()) {
		
		SBRE_set_clear_color((Color){ 67.5, 25, 122.5, 255 });
		SBRE_clear();


		SBRE_draw_quad(SBRE_VEC2(50.0f, 50.0f), 50.0f, 50.0f, SBRE_COLOR(123, 50, 255, 255));

		SBRE_draw_quad_outline(SBRE_VEC2(200.0f, 200.0f), 50.0f, 50.0f, 2.0f, SBRE_COLOR(123, 50, 255, 255), SBRE_WHITE);


		SBRE_display();
		SBRE_poll_events();
	}
	
	
	SBRE_terminate();
	return 0;
}
