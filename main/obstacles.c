#include "obstacles.h"

const int red_barrel_seq[] = {KC_BTN_RIGHT, KC_BTN_RIGHT, KC_BTN_RIGHT};
const int green_barrel_seq[] = {KC_BTN_RIGHT, KC_BTN_RIGHT, KC_BTN_UP};
const int blue_barrel_seq[] = {KC_BTN_RIGHT, KC_BTN_RIGHT, KC_BTN_LEFT};

const Obstacle red_barrel_obstacle = {.tile_map=&map_barrel_red_Tile_Layer_1, .patterns=red_barrel_seq};
const Obstacle green_barrel_obstacle = {.tile_map=&map_barrel_green_Tile_Layer_1, .patterns=green_barrel_seq};
const Obstacle blue_barrel_obstacle = {.tile_map=&map_barrel_blue_Tile_Layer_1, .patterns=blue_barrel_seq};