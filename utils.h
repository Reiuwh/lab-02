#ifndef UTILS_H
#define UTILS_H

#define Inventory_size 10
#define MAX_ITEMS  10
#define MAX_NAME_LEN 32
#define EMPTY_ITEM 0

extern char item_names[MAX_ITEMS][MAX_NAME_LEN];
extern char farmer_name[MAX_NAME_LEN];
extern int inventory[Inventory_size] ;
extern int current_day;
extern int current_hour ;

void clear_input();
int safe_input_int(const char* prompt);
void safe_input_string(const char* prompt, char* buffer, int max_len);
void load_items(const char* filename);
void show_time(int day, int hour);
void skip_time(int* day, int* hour);
void write_diary();
void process_letter();

#endif