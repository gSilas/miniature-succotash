#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "appfs.h"
#include <assert.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "ugui.h"
#include "8bkc-hal.h"
#include "8bkc-ugui.h"
#include "tilegfx.h"
#include "appfs.h"
#include "rom/rtc.h"
#include "sndmixer.h"
#include "esp_timer.h"
#include <math.h>
#include "powerbtn_menu.h"
#include "graphics.h"

#define REFRESH_RATE (50)

#define SCREEN_W (160)
#define SCREEN_H (128)

#define CELL_W (8)
#define CELL_H (8)

#define PLAYER_W (32)
#define PLAYER_H (64)

#define INPUT_W (32)
#define INPUT_H (32)

#define OBSTACLE_W (32)
#define OBSTACLE_H (32) 

#define FRAME_DELAY_USEC (250000)

int get_keydown() {
	static int oldBtns=0xffff;
	int newBtns=kchal_get_keys();
	int ret=(oldBtns^newBtns)&newBtns;
	oldBtns=newBtns;
	return ret;
}

bool animate_tile(bool get) {
	static bool tile = 0;

	if (get) return tile;

	tile = !tile;
	printf("switch %d\n", tile);

	return 0;
}

void app_main() {
	kchal_init(); //Initialize the PocketSprite SDK.
	tilegfx_init(1, REFRESH_RATE); //Initialize TileGFX, Doublesized mode, 50FPS

	esp_timer_create_args_t tile_switch_timer_args = {
		.callback = &animate_tile,
		.arg = (void*) false,
		.name = "tile_switch_timer"
	};

	esp_timer_handle_t tile_switch_timer;
	esp_timer_create(&tile_switch_timer_args, &tile_switch_timer);

	tilegfx_rect_t input_trect={.h=INPUT_H, .w=INPUT_W, .x=0, .y=0};
	tilegfx_rect_t playstate_trect={.h=INPUT_H, .w=INPUT_W, .x=SCREEN_W-INPUT_W, .y=0};
	tilegfx_rect_t player_trect={.h=PLAYER_H, .w=PLAYER_W, .x=(SCREEN_W-PLAYER_W)/4, .y=SCREEN_H - PLAYER_H};
	tilegfx_rect_t press_start_trect={.h=INPUT_H, .w=INPUT_W*3, .x=(SCREEN_W-INPUT_W)/2, .y=(SCREEN_H - INPUT_H)/2};
	tilegfx_rect_t obstacle_1_trect={.h=OBSTACLE_H, .w=OBSTACLE_W, .x=0, .y=(SCREEN_H-OBSTACLE_H)};
	tilegfx_rect_t obstacle_2_trect={.h=OBSTACLE_H, .w=OBSTACLE_W, .x=(SCREEN_W-OBSTACLE_W), .y=(SCREEN_H-OBSTACLE_H)};

	const tilegfx_map_t* render_btn_map = &map_a_btn_Tile_Layer_1;
	const tilegfx_map_t* render_playstate_map = &map_start_btn_pause_Tile_Layer_1;
	const tilegfx_map_t* render_player_map = &map_running_man_idle_1_Tile_Layer_1;
	const tilegfx_map_t* render_obstacle_1_map = &map_barrel_red_Tile_Layer_1;
	const tilegfx_map_t* render_obstacle_2_map = &map_barrel_green_Tile_Layer_1;

	esp_timer_start_periodic(tile_switch_timer, FRAME_DELAY_USEC);

	while(1) {
		int btn = get_keydown();

		printf("IDLE %d\n", btn);

		if (btn&KC_BTN_START) {
			render_playstate_map = &map_start_btn_play_Tile_Layer_1;
			break;
		}

		if(animate_tile(true)){
			render_player_map = &map_running_man_idle_1_Tile_Layer_1;
		} 
		else {
			render_player_map = &map_running_man_idle_2_Tile_Layer_1;
		} 

		tilegfx_fade(255, 255, 255, 0);
		tilegfx_tile_map_render(render_playstate_map, 0, 0, &playstate_trect);
		tilegfx_tile_map_render(&map_start_game_Tile_Layer_1, 0, 0, &press_start_trect);
		tilegfx_tile_map_render(render_player_map, 0, 0, &player_trect);
		tilegfx_flush();
	}

	while(1) {
		int btn = get_keydown();

		printf("PLAY %d\n", btn);

		if (btn&KC_BTN_UP) {
			render_btn_map = &map_up_btn_Tile_Layer_1;
		}
		else if (btn&KC_BTN_DOWN) {
			render_btn_map = &map_down_btn_Tile_Layer_1;
		}
		else if (btn&KC_BTN_LEFT) {
			render_btn_map = &map_left_btn_Tile_Layer_1;
		}
		else if (btn&KC_BTN_RIGHT) {
			render_btn_map = &map_right_btn_Tile_Layer_1;
		}
		else if (btn&KC_BTN_A) render_btn_map = &map_a_btn_Tile_Layer_1;
		else if (btn&KC_BTN_B) render_btn_map = &map_b_btn_Tile_Layer_1;
		else if (btn&KC_BTN_START) {
			render_playstate_map = &map_start_btn_pause_Tile_Layer_1;
			
			tilegfx_fade(255, 255, 255, 0);
			tilegfx_tile_map_render(render_playstate_map, 0, 0, &playstate_trect);
			tilegfx_flush();

			while(1){
				int btn = get_keydown();

				printf("PAUSE %d\n", btn);

				if (btn&KC_BTN_START) {
					render_playstate_map = &map_start_btn_play_Tile_Layer_1;
					break;
				}
			}
		}

		if(animate_tile(true)){
			render_player_map = &map_running_man_run_1_Tile_Layer_1;
		} 
		else {
			render_player_map = &map_running_man_run_2_Tile_Layer_1;
		} 

		tilegfx_fade(255, 255, 255, 0);
		tilegfx_tile_map_render(render_btn_map, 0, 0, &input_trect);
		tilegfx_tile_map_render(render_playstate_map, 0, 0, &playstate_trect);
		tilegfx_tile_map_render(render_player_map, 0, 0, &player_trect);
		tilegfx_tile_map_render(render_obstacle_1_map, 0, 0, &obstacle_1_trect);
		tilegfx_tile_map_render(render_obstacle_2_map, 0, 0, &obstacle_2_trect);
		tilegfx_flush();
	}
}
