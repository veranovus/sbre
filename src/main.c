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


	Font* font = SBRE_create_font("../res/font/arial/arial.ttf", 24, SBRE_LINEAR);


	Vec2 pos = SBRE_VEC2(0, 0);
	float speed = 100;


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


		if (SBRE_get_mouse_button_pressed(0))
			printf("[INFO][Mouse button 0 just pressed.]\n");
		else if (SBRE_get_mouse_button_released(0))
			printf("[INFO][Mouse button 0 just released.]\n");
		else if (SBRE_get_mouse_button(0))
			printf("[INFO][Mouse button 0 is still pressed.]\n");
		

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


		for (int i = 0; i < 1; ++i) {

			SBRE_draw_quad((Vec2){5 + i % (SCREEN_WIDTH / 15) * 15, 5 + i / (SCREEN_WIDTH / 15) * 15}, 10, 10, SBRE_BLUE);
		}

		SBRE_draw_text((Vec2){0, 0}, fps_text, font, SBRE_WHITE);


		SBRE_display();
		SBRE_poll_events();
	}

	SBRE_free_font(font);

	SBRE_terminate();
	return 0;
}
