#ifndef UTILS_H
#define UTILS_H

#define Inventory_size 10
#define MAX_ITEMS  10
#define MAX_NAME_LEN 32
#define EMPTY_ITEM 0

struct GameState {
	struct Player* player;
	int current_day;
	int current_hour;
	char item_names[MAX_ITEMS][MAX_NAME_LEN];
};


void clear_input();
int safe_input_int(const char* prompt);
void safe_input_string(const char* prompt, char* buffer, int max_len);
void load_items(const char* filename, char item_names[MAX_ITEMS][MAX_NAME_LEN]);
void show_time(int day, int hour);
void skip_time(int* day, int* hour);
void write_diary(struct GameState* game);
void process_letter(struct GameState* game);


#endif