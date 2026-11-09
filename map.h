#ifndef MAP_H
#define MAP_H
#include "player.h"

#define MIN_MAP_SIZE 5
#define MAX_MAP_SIZE 30


void free_map(char** map, int height);
char** generate_dungeon(int* height, int* width, int* hero_x, int* hero_y, int* stairs_down_x, int* stairs_down_y, int* stairs_up_x, int* stairs_up_y);
void print_map(char** map, int height, int width, int depth);
int is_blocked(char** map, int height, int width, int x, int y);
void start_adventure();

#endif