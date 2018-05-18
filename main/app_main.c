#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "appfs.h"
#include <assert.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
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

#define MAP_W (20)
#define MAP_H (16)

#define CELL_W (8)
#define CELL_H (8)

#define TILE_W (4)
#define TILE_H (4)

int get_keydown() {
	static int oldBtns=0xffff;
	int newBtns=kchal_get_keys();
	int ret=(oldBtns^newBtns)&newBtns;
	oldBtns=newBtns;
	return ret;
}

void app_main() {
	kchal_init(); //Initialize the PocketSprite SDK.
	tilegfx_init(1, 50); //Initialize TileGFX, Doublesized mode, 50FPS

	tilegfx_rect_t input_trect={.h=TILE_H*CELL_H, .w=TILE_W*CELL_W, .x=0, .y=0};
	tilegfx_rect_t player_trect={.h=TILE_H*CELL_H, .w=TILE_W*CELL_W, .x=0, .y=0};
	
	const tilegfx_map_t* render_btn_map;

	render_btn_map = &map_up_btn_Tile_Layer_1;

	while(1) {
		tilegfx_fade(47, 72, 78, 0);

		int btn = kchal_get_keys();

		printf("%d\n", btn);

		if (btn&KC_BTN_UP) {
			render_btn_map = &map_up_btn_Tile_Layer_1;
			if ((player_trect.y - CELL_H) > 0 - CELL_H) player_trect.y -= CELL_H;
		}
		else if (btn&KC_BTN_DOWN) {
			render_btn_map = &map_down_btn_Tile_Layer_1;
			if ((player_trect.y + CELL_H) < CELL_H * MAP_H - CELL_H * TILE_H + CELL_H) player_trect.y += CELL_H;
		}
		else if (btn&KC_BTN_LEFT) {
			render_btn_map = &map_left_btn_Tile_Layer_1;
			if ((player_trect.x + CELL_W) > 0 + CELL_W) player_trect.x -= CELL_W;
		}
		else if (btn&KC_BTN_RIGHT) {
			render_btn_map = &map_right_btn_Tile_Layer_1;
			if ((player_trect.x + CELL_W) < CELL_W * MAP_W - CELL_W * TILE_W + CELL_W) player_trect.x += CELL_W;
		}
		else if (btn&KC_BTN_A) render_btn_map = &map_a_btn_Tile_Layer_1;
		else if (btn&KC_BTN_B) render_btn_map = &map_b_btn_Tile_Layer_1;
		else if (btn&KC_BTN_START) render_btn_map = &map_start_btn_Tile_Layer_1;
		else if (btn&KC_BTN_SELECT) render_btn_map = &map_select_btn_Tile_Layer_1;

		tilegfx_tile_map_render(render_btn_map, 0, 0, &input_trect);
		tilegfx_tile_map_render(&map_player_wizard_Tile_Layer_1, 0, 0, &player_trect);

		tilegfx_flush();
	}
}
