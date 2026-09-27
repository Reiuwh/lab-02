#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define Inventory_size 10
//защита от дурака
void clear_input() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}
int safe_input_int(const char* prompt) {
	int value;
	int result;

	do {
		printf("%s", prompt);
		result = scanf_s("%d", &value); // пытаемся прочитать число

		if (result != 1) {
			printf("Ошибка!Введите число.\n");
			clear_input();
		}
	} while (result != 1);
	return value;
}
int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	int current_day = 1;                        //вводим переменные
	int current_hour = 8;
	int hours_to_work = 0;
	int predmet = 0;
	int slot = 0;
	int item_to_drop = 0;
	int inventory[Inventory_size] = { 0,1,2,3,0,0,0,0,0,0 };
	const char* names[Inventory_size] = {
		"empty_item",
		"wood",
		"stone",
		"seeds",
		"coal",
		"apple",
		"can",
		"flower",
		"meat",
		"pickaxe" };
}