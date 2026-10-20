#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#define Inventory_size 10
#define MAX_ITEMS  10
#define MAX_NAME_LEN 32

char item_names[MAX_ITEMS][MAX_NAME_LEN]; //ГЛОБАЛЬНЫЕ ПЕРЕМЕННЫЕ
char farmer_name[MAX_NAME_LEN];
int inventory[Inventory_size] = { 0,1,2,3,0,1,0,1,0,0 };
int current_day = 1;                        
int current_hour = 8;
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
//ФУНКЦИЯ БЕЗОПАСНОГО ВВОДА СТРОКИ
void safe_input_string(const char* prompt, char* buffer, int max_len) { //читаем стороку целиком(с пробелами) убирая enter в конце 
	printf("%s", prompt); //печатаем подсказку

	if (fgets(buffer, max_len, stdin) == NULL) {
		buffer[0] = '\0'; //если ошибка делаем строку пустой
		return;
	}
	// fgets оставляет '\n' в конце строки нужно его убрать
	int len = strlen(buffer);
	if (len > 0 && buffer[len - 1] == '\n') {
		buffer[len - 1] = '\0'; //заменяет enter на конец строки
	}
}
void load_items(const char* filename) {
	// Сначала заполним массив названиями по умолчанию
	// (на случай, если файл не найдётся)
	strcpy(item_names[0], "Пусто");
	strcpy(item_names[1], "Дерево");
	strcpy(item_names[2], "Камень");
	strcpy(item_names[3], "Семена");
	strcpy(item_names[4], "Железо");
	strcpy(item_names[5], "Золото");
	strcpy(item_names[6], "Алмаз");
	strcpy(item_names[7], "Уголь");
	strcpy(item_names[8], "Палка");
	strcpy(item_names[9], "Веревка");

	FILE * file = fopen(filename, "r");

	// Если файл не открылся - сообщаем и выходим из функции
	if (file == NULL) {
		printf("»> Файл %s не найден. Использую названия по умолчанию.\n", filename);
		return;  // Выходим, но названия по умолчанию уже стоят
	}

	// Читаем файл построчно
	char line[256];  // Буфер для одной строки
	while (fgets(line, sizeof(line), file) != NULL) {
		int id;                  // ID предмета (число)
		char name[MAX_NAME_LEN]; // Название предмета

		// sscanf разбирает строку формата "0 Пусто" на два значения
		// %d - читает число, %31[^\n] - читает строку до конца строки (до Enter)
		if (sscanf(line, "%d %31[^\n]", &id, name) == 2) {
			// Проверяем, что ID в допустимых пределах
			if (id >= 0 && id < MAX_ITEMS) {
				strcpy(item_names[id], name);  // Копируем название в каталог
			}
		}
	}

	fclose(file); 
	printf("»> Названия предметов загружены из %s\n", filename);
}

// ФУНКЦИЯ ВЫВОДА ИНВЕНТАРЯ
void print_inventory() {
	printf("--- Содержимое рюкзака ---\n");

	// Проходим по всем 10 слотам
	for (int i = 0; i < Inventory_size; i++) {
		// inventory[i] - это ID предмета в слоте i
		// item_names[ID] - это название предмета с таким ID
		printf("Слот %d: [%d] - %s\n", i, inventory[i], item_names[inventory[i]]);
	}
}

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
			print_inventory();
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
		default:
			printf("Ошибка! Выберите пункт меню 1-6 \n");
			break;
		}
	}
	return 0;
}