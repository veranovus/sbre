#include "sbre.h"


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TITLE "C OpenGL"
#define VSync 0
#define QUAD_SIZE 50



int main(void) {
	

	if (!SBRE_init(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE, VSync))
		return -1;

	
	while(!SBRE_window_should_close()) {
		
		SBRE_set_clear_color(SBRE_BLACK);
		SBRE_clear();


		SBRE_draw_quad(SBRE_VEC2((SCREEN_WIDTH - QUAD_SIZE) / 2, (SCREEN_HEIGHT - QUAD_SIZE) / 2), QUAD_SIZE, QUAD_SIZE, SBRE_COLOR(123, 50, 255, 255));


		SBRE_display();
		SBRE_poll_events();
	}
	
	
	SBRE_terminate();
	return 0;
}
