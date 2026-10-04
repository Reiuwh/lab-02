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
	int target_id = 0;
	int inventory[Inventory_size] = { 0,1,2,3,0,1,0,1,0,0 };
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
	int choice = -1;

	while (1) {
		printf("\n === МЕНЮ ФЕРМЕРА ===\n"
			"[0] Выход\n"
			"[1] Посмотреть на часы\n"
			"[2] Промотать время(поработать)\n"
			"[3] Посмотреть инвентарь\n"
			"[4] Положить предмет в слот\n"
			"[5] Выбросить предмет\n"
			"[6] Отчистка от мусора\n");


		choice = safe_input_int("Выберите пункт:");
		switch (choice) {
		case 0:
			printf("Выход из игры.До скорой встречи!\n");
			return 0;
		case 1:
			printf("Текущее время: День %d , %02d:00\n", current_day, current_hour);
			break;
		case 2:
			hours_to_work = safe_input_int("Сколько часов вы хотите поработать?");

			if (hours_to_work < 0) {
				printf("Ошибка:Количество часов не может быть отрицательным!\n");
				break;
			}

			current_hour += hours_to_work;
			while (current_hour >= 24) {
				current_hour -= 24;
				current_day++;
			}
			printf("Вы успешно поработали! Текущее время : День %d,%02d:00\n", current_day, current_hour);
			break;

		case 3:
			printf("Содержимое рюкзака\n");
			for (int i = 0; i < Inventory_size; i++) {
				printf("Слот %d:[%s]", i, names[inventory[i]]);
			}
			break;
		case 4:
			printf("Введите номер предмета:\n");
			scanf_s("%d", &predmet);
			printf("Введите номер слота:\n");
			scanf_s("%d", &slot);
			if (predmet && slot >= 0 && predmet && slot < 10) {
				inventory[slot] = predmet;
			}

			else {
				printf("Введите число от 1 до 9!\n");
			}
		case 5:
			item_to_drop = safe_input_int("Введите id предмета:\n");

			if (item_to_drop <= 0 || item_to_drop >= Inventory_size) {
				printf("Ошибка!Такого слота не существует!Введите от 1 до %d.\n", Inventory_size - 1);
				break;
			}
			inventory[item_to_drop] = "empty_item";
			printf("Слот %d очищен.\n", item_to_drop);
			break;

		case 6:
			target_id = safe_input_int("Введите id предмета для отчистки:");
			if (target_id <= 0 || target_id >= Inventory_size) {
				printf("Ошибка!Неверный  id предмета.\n");
				break;
			}
			int clean_count = 0;

			for (int i = 0; i < Inventory_size; i++) {
				if (inventory[i] == target_id) {
					inventory[i] = "empty_item";
					clean_count++;
				}
			}
			if (clean_count > 0) {
				printf("Очищено слотов: %d\n", clean_count);
			}
			else {
				printf("Предмет с тким id не найден в инвентаре.\n");
			}
				break;
			
		}
	}
}