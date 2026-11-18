#ifndef PLAYER_H
#define PLAYER_H
#include "utils.h"
#define START_HP 100
#define START_STAMINA 100
#define START_GOLD 10

extern int inventory[Inventory_size];

void print_inventory();
void show_inventory(int inv[], char names[][MAX_NAME_LEN]);
void put_item(int inv[]);
void discard_item(int inv[]);
void search_item_by_name();
void clean_trash(int inv[]);
void destroy_player(struct Player* player);

struct Player {
	char farmer_name[MAX_NAME_LEN];
	int hp;
	int max_hp;
	int stamina;
	int max_stamina;
	int gold;
	int x, y;
	int inventory[Inventory_size];
	int map_count;
};

struct Player* create_player(const char* name);
struct Player* recreate_player(struct Player* player);

#endif