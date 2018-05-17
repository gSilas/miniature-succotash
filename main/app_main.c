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

#define MAP_W (40) //tiles, width of pipemap
#define MAP_H (16)

#define TILE_W (8)
#define TILE_H (8)

#define A_BTN_TILE (64)

tilegfx_map_t *bgn_map;

void app_main() {
	kchal_init(); //Initialize the PocketSprite SDK.
	tilegfx_init(1, 50); //Initialize TileGFX, Doublesized mode, 50FPS
	sndmixer_init(2, 22050); //Initialize sound mixer. 2 channels at 22050Hz sample rate.
	//Create and clear the tilemap containing the dynamically shown pipes.
	bgn_map = tilegfx_create_tilemap(1, 1, &tileset_sheet);
	tilegfx_set_tile(bgn_map, 0, 0, 95);

	tilegfx_rect_t trect={.h=4*8, .w=4*8, .x=0, .y=0};

	while(1) {
		//Start a game
		tilegfx_fade(255, 0, 255, 0);
		tilegfx_tile_map_render(bgn_map, 0, 0, NULL);
		tilegfx_tile_map_render(&map_a_btn_Tile_Layer_1, 0, 0, &trect);
		tilegfx_flush();
	}
}
