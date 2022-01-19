#include "sbre.h"


#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define TITLE "SBRE"
#define VSync 0



int main(void) {
	
	if (!SBRE_init(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE, VSync))
		return -1;


	double delta_time;
	char fps_text[12];


	Vec2 pos = SBRE_VEC2(0, 0);
	float speed = 100;


	Font* font = SBRE_create_font("../res/font/press_start_2p/PressStart2P.ttf", 18, SBRE_NEAREST);

	Texture* t = SBRE_load_texture("../res/sprite_sheet.png", SBRE_NEAREST);
	t->width  = 64;
	t->height = 64;


	float rotation = 30;


	while(!SBRE_window_should_close()) {

		SBRE_calculate_delta_time();
		SBRE_calculate_fps();

		delta_time = SBRE_delta_time();
		uint32_t fps = SBRE_fps();
		snprintf(fps_text, 12, "FPS : %d", fps);

		const SBRE_CharInput* char_pressed = SBRE_get_chars_pressed();
		const SBRE_KeyInput* keys_pressed = SBRE_get_keys_pressed();

		for (int i = 0; i < keys_pressed->input_count; ++i) {

			if (keys_pressed->keys_pressed[i].key == SBRE_KEY_BACKSPACE) {

				printf("Key : Backspace, Action : %d\n", keys_pressed->keys_pressed[i].action_type);
			}
			else {
				
				printf("Key : %c, Action : %d\n", keys_pressed->keys_pressed[i].key, keys_pressed->keys_pressed[i].action_type);
			}
		}
		

		rotation += 0.005;


		if 		(SBRE_get_key_press(SBRE_KEY_A))
			pos.x -= speed * delta_time;
		else if (SBRE_get_key_press(SBRE_KEY_D))
			pos.x += speed * delta_time;
		if 		(SBRE_get_key_press(SBRE_KEY_W))
			pos.y -= speed * delta_time;
		else if (SBRE_get_key_press(SBRE_KEY_S))
			pos.y += speed * delta_time;


		SBRE_set_clear_color((Color){ 12.3, 2.5, 25, 255 });
		SBRE_clear();


		SBRE_begin_batch();

		SBRE_batch_render_line(SBRE_VEC2(780, 500), SBRE_VEC2(50, 100), 1, SBRE_WHITE);

		SBRE_batch_render_quad_outline(SBRE_VEC2(150, 150), 50, 50, 2, SBRE_RED, SBRE_WHITE);
		
		SBRE_batch_render_quad_outline_ext(SBRE_VEC2(200, 150), 50, 50, 2, rotation, SBRE_RED, SBRE_WHITE);


		SBRE_batch_render_text(SBRE_VEC2(5, 5), fps_text, font, SBRE_WHITE);

		Vec2 size = SBRE_font_get_text_size(font, fps_text);
		SBRE_batch_render_quad_outline(SBRE_VEC2(5, 5), size.x, size.y, 1, SBRE_TRANSPARENT, SBRE_GREEN);


		SBRE_end_batch();
		SBRE_render_batch(true);


		SBRE_display();
		SBRE_poll_events();
	}
	

	/* Cleanup */
	SBRE_free_font(font);
	SBRE_free_texture(t);


	SBRE_terminate();
	return 0;
}
