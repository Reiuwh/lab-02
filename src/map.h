#ifndef MAP_H
#define MAP_H
#include "player.h"

#define MIN_MAP_SIZE 5
#define MAX_MAP_SIZE 30


void free_map(struct Map* map);
struct Map* generate_dungeon(int depth);
void print_map(struct Map* map, struct GameState* game);
int is_blocked(struct Map* map, int x, int y);
void start_adventure(struct GameState* game);

enum Biome {
	BIOME_FOREST = 0,
	BIOME_SWAMP,
	BIOME_RUINS,
	BIOME_CRYSTAL
};

struct Map {
	char** field;
	int width;
	int height;
	int depth;
	enum Biome biome;

	int hero_x;
	int hero_y;

	int stairs_down_x;
	int stairs_down_y;
	int stairs_up_x;
	int stairs_up_y;

};

#endif