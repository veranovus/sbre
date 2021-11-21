#include "sbre.h"


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TITLE "SBRE"
#define VSync 0



int main(void) {
	
	if (!SBRE_init(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE, VSync))
		return -1;


	double delta_time;


	Vec2 pos = SBRE_VEC2(0, 0);
	float speed = 100;

	/* 
	 * TODO : I managed to render every glyph available correctly, now only thing left to do is
	 *		  implementing batch rendering for text.
	 */
	Texture* t = SBRE_test_freetype();

	SBRE_Character ch = get_character('c');
	float m = ch.offset.y;

	t->width  = ch.size.x * 2;
	t->height = ch.size.y * 2;

	
	while(!SBRE_window_should_close()) {

		SBRE_calculate_delta_time();
		SBRE_calculate_fps();

		delta_time = SBRE_delta_time();


		if 		(SBRE_get_key_press(SBRE_KEY_A))
			pos.x -= speed * delta_time;
		else if (SBRE_get_key_press(SBRE_KEY_D))
			pos.x += speed * delta_time;
		if 		(SBRE_get_key_press(SBRE_KEY_W))
			pos.y -= speed * delta_time;
		else if (SBRE_get_key_press(SBRE_KEY_S))
			pos.y += speed * delta_time;


		SBRE_set_clear_color((Color){ 67.5, 25, 122.5, 255 });
		SBRE_clear();


		//SBRE_draw_texture(SBRE_VEC2(0, 0), t, NULL);
		SBRE_draw_char(SBRE_VEC2(50, 50), t, &(Rectangle) { SBRE_VEC2(ch.offset.x, ch.offset.y), ch.size.x, ch.size.y });

		/*
		SBRE_draw_quad(SBRE_VEC2(50.0f, 50.0f), 50.0f, 50.0f, SBRE_COLOR(123, 50, 255, 255));

		SBRE_draw_quad_outline(pos, 50.0f, 50.0f, 2.0f, SBRE_COLOR(123, 50, 255, 255), SBRE_WHITE);

		SBRE_draw_circle(SBRE_VEC2(200.0f, 200.0f), 30.0f, SBRE_WHITE);

		SBRE_draw_circle_outline(SBRE_VEC2(300.0f, 200.0f), 30.0f, 0.1f, SBRE_RED, SBRE_WHITE);
		*/

		SBRE_display();
		SBRE_poll_events();
	}
	
	
	SBRE_terminate();
	return 0;
}
