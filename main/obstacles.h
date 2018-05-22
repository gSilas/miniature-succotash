#pragma once

#include "8bkc-hal.h"
#include "tilegfx.h"
#include "graphics.h"

#define NUM_OBSTACLES (3)
#define PATTERN_LENGTH (3)

typedef struct {
	const tilegfx_map_t* tile_map;	
	const int* patterns;
} Obstacle;

extern const Obstacle red_barrel_obstacle;
extern const Obstacle green_barrel_obstacle;
extern const Obstacle blue_barrel_obstacle;