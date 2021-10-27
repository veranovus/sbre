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
		
		SBRE_set_clear_color((Color){ 0.2f, 0.1f, 0.5f, 1.0f });
		SBRE_clear();


		SBRE_draw_quad((Vec2){ 50.0f, 50.0f }, 50.0f, 50.0f, (Color){ 0.4f, 0.1f, 1.0f, 1.0f });


		SBRE_display();
		SBRE_poll_events();
	}
	
	
	SBRE_terminate();
	return 0;
}
