#ifndef PLAYER_H
#define PLAYER_H
#include "utils.h"

extern int inventory[Inventory_size];

void print_inventory();
void show_inventory(int inv[], char names[][MAX_NAME_LEN]);
void put_item(int inv[]);
void discard_item(int inv[]);
void search_item_by_name();
void clean_trash(int inv[]);

#endif