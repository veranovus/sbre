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


	Font* font = SBRE_create_font("../res/font/press_start_2p/PressStart2P.ttf", 18, SBRE_NEAREST);

	Texture* t = SBRE_load_texture("../res/sprite_sheet.png", SBRE_NEAREST);
	t->width  = 64;
	t->height = 64;

	
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

		
		SBRE_draw_quad(SBRE_VEC2(50.0f, 50.0f), 50.0f, 50.0f, SBRE_COLOR(123, 50, 255, 255));

		SBRE_draw_quad_outline(pos, 50.0f, 50.0f, 2.0f, SBRE_COLOR(123, 50, 255, 255), SBRE_WHITE);

		SBRE_draw_circle(SBRE_VEC2(200.0f, 200.0f), 30.0f, SBRE_WHITE);

		SBRE_draw_circle_outline(SBRE_VEC2(300.0f, 200.0f), 30.0f, 0.1f, SBRE_RED, SBRE_WHITE);


		SBRE_draw_text(SBRE_VEC2(50.0f, 50.0f), "Hello text!", font, SBRE_WHITE);

		SBRE_draw_texture(SBRE_VEC2(100.0f, 350.0f), t, NULL);

		SBRE_draw_text(SBRE_VEC2(50.0f, 100.0f), "Phyton huh? C is way better.", font, SBRE_GREEN);


		SBRE_display();
		SBRE_poll_events();
	}
	

	/* Cleanup */
	SBRE_free_font(font);


	SBRE_terminate();
	return 0;
}
