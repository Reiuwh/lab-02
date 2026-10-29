#include "functions.h"


int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	int hours_to_work = 0;
	int predmet = 0;
	int slot = 0;
	int item_to_drop = 0;
	int target_id = 0;


	load_items("items.txt");

	printf("Введите имя фермера: ");
	safe_input_string("", farmer_name, MAX_NAME_LEN);
	// Если имя пустое - ставим "Фермер"
	if (strlen(farmer_name) == 0) {
		strcpy(farmer_name, "Фермер");
	}

	int choice = -1;

	// Если имя пустое - ставим "Фермер"
	if (strlen(farmer_name) == 0) {
		strcpy(farmer_name, "Фермер");
	}

	while (1) {
		printf("\n === МЕНЮ ФЕРМЕРА ===\n");
		printf("Привет, %s! \n ", farmer_name);
		printf("[0] Выход\n"
			"[1] Посмотреть на часы\n"
			"[2] Промотать время(поработать)\n"
			"[3] Посмотреть инвентарь\n"
			"[4] Положить предмет в слот\n"
			"[5] Выбросить предмет\n"
			"[6] Отчистка от мусора\n"
			"[7] Поиск предмета по названию\n"
			"[8] Записать состояние в дневник\n"
			"[9] Обработать письмо NPC\n");

		choice = safe_input_int("Выберите пункт:");
		switch (choice) {
		case 0:
			printf("Выход из игры.До скорой встречи!\n");
			return 0;
		case 1:
			show_time(current_day, current_hour);
			break;
		case 2:
			skip_time(&current_day, &current_hour);
			break;
			
		case 3:
			show_inventory(inventory, item_names);
			break;

		case 4:
			put_item(inventory);
			break;

		case 5:
			discard_item(inventory);
			break;

		case 6:
			clean_trash(inventory);
			break;

		case 7:
			search_item_by_name(inventory,item_names);
			break;

		case 8:
			write_diary(farmer_name, current_day, current_hour, inventory);
			break;

		case 9:
			process_letter(current_day, inventory);
			break;

		default:
			printf(" Такого пункта нет. Попробуйте снова.\n");
			break;
		}


	}
	return 0;
}