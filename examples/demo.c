#include <SBRE/sbre.h>


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TITLE "SBRE Demo"
#define VSync 0
#define QUAD_SIZE 50



int main(void) {
	
	/* Initialize SBRE and subsystems, calling this function more than once may result in undefined behaviour. */
	if (!SBRE_init(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE, VSync))
		return -1;

	
	while(!SBRE_window_should_close()) {

		/* Sets the clear color given color */
		SBRE_set_clear_color(SBRE_BLACK);		
		/* Clears the display */
		SBRE_clear();


		/* Draws a quad with the given parameters */
		SBRE_draw_quad(SBRE_VEC2((SCREEN_WIDTH - QUAD_SIZE) / 2, (SCREEN_HEIGHT - QUAD_SIZE) / 2), QUAD_SIZE, QUAD_SIZE, SBRE_COLOR(123, 50, 255, 255));


		/* Displays the contents which has been drawn since the last call */ 
		SBRE_display();
		/* Poll events for the current window */
		SBRE_poll_events();
	}
	
	/* Terminate the SBRE and subsystmes, calling this function more than once may result in undefined behaviour. */
	SBRE_terminate();
	return 0;
}
