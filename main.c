#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#define Inventory_size 10
#define MAX_ITEMS  10
#define MAX_NAME_LEN 32
#define EMPTY_ITEM 0

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

	FILE* file = fopen(filename, "r");

	// Если файл не открылся - сообщаем и выходим из функции
	if (file == NULL) {
		printf(" Файл %s не найден. Использую названия по умолчанию.\n", filename);
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
	printf(" Названия предметов загружены из %s\n", filename);
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
void show_time(int day, int hour) {
	printf("Текущее время: День %d , %02d:00\n", day, hour);
}

void skip_time(int* day, int* hour) {
	int hours = safe_input_int("Сколько часов вы хотите поработать? ");
	if (hours < 0) {
		printf(" Ошибка: количество часов не может быть отрицательным!\n");
		return;
	}
	*hour += hours;
	while (*hour >= 24) {
		*hour -= 24;
		(*day)++;
	}
	printf("Вы поработали! Теперь: День %d, %02d:00\n", *day, *hour);
}

void show_inventory(int inv[], char names[][MAX_NAME_LEN]) {
	printf("--- Содержимое рюкзака ---\n");
	for (int i = 0; i < Inventory_size; i++) {
		printf("Слот %d: [%d] - %s\n", i, inv[i], names[inv[i]]);
	}
}

void put_item(int inv[]) {
	int index = safe_input_int("Введите индекс слота (0-9): ");
	if (index < 0 || index >= Inventory_size) {
		printf("Ошибка: Индекс должен быть от 0 до %d.\n", Inventory_size - 1);
		return;
	}
	int item_id = safe_input_int("Введите ID предмета: ");
	inv[index] = item_id;
	printf("Предмет успешно положен в слот %d.\n", index);
}

void discard_item(int inv[]) {
	int index = safe_input_int("Введите индекс слота для очистки (0-9): ");
	if (index < 0 || index >= Inventory_size) {
		printf(" Ошибка: Индекс должен быть от 0 до %d.\n", Inventory_size - 1);
		return;
	}
	inv[index] = EMPTY_ITEM;
	printf("Слот %d очищен.\n", index);
}

void clean_trash(int inv[]) {
	int target_id = safe_input_int("Введите ID предмета для очистки: ");
	if (target_id == EMPTY_ITEM) {
		printf(" Ошибка: Нельзя очищать пустые слоты!\n");
		return;
	}
	int clean_count = 0;
	for (int i = 0; i < Inventory_size; i++) {
		if (inv[i] == target_id) {
			inv[i] = EMPTY_ITEM;
			clean_count++;
		}
	}
	if (clean_count > 0) {
		printf("Очищено слотов: %d\n", clean_count);
	}
	else {
		printf("Предмет с таким ID не найден.\n");
	}
}

// ФУНКЦИЯ ПОИСКА ПРЕДМЕТА ПО НАЗВАНИЮ(Пункт 7)
void search_item_by_name() {
	char search_name[MAX_NAME_LEN];// Сюда запишем название для поиска
	getchar();


	safe_input_string("Введите название предмета: ", search_name, MAX_NAME_LEN);

	// Ищем ID, которому соответствует это название
	int target_id = -1;  // -1 означает "не найдено"
	for (int i = 0; i < MAX_ITEMS; i++) {
		// strcmp сравнивает строки. Если равны - возвращает 0.
		if (strcmp(item_names[i], search_name) == 0) {
			target_id = i;  // Запоминаем ID
			break;          // Выходим из цикла (нашли)
		}
	}

	// Если не нашли название в каталоге
	if (target_id == -1) {
		printf("Предмет с названием '%s' не найден в каталоге.\n", search_name);
		return;
	}

	// Ищем этот ID в рюкзаке
	printf("Предмет '%s' (ID: %d) найден в слотах: ", search_name, target_id);
	int found = 0;  // Флаг: нашли ли хоть один слот
	for (int i = 0; i < Inventory_size; i++) {
		if (inventory[i] == target_id) {
			printf("%d ", i);  // Печатаем номер слота
			found = 1;         // Запоминаем, что нашли
		}
	}

	if (found == 0) {
		printf("(нет в рюкзаке)");
	}
	printf("\n");
}

// ФУНКЦИЯ ЗАПИСИ В ДНЕВНИК (Пункт 8)

void write_diary() {
	
	FILE* file = fopen("diary.txt", "a");

	if (file == NULL) {
		printf(" Ошибка: не удалось открыть diary.txt\n");
		return;
	}

	// Записываем сводку в файл
	fprintf(file, "=== Запись в дневнике ===\n");
	fprintf(file, "Фермер: %s\n", farmer_name);
	fprintf(file, "День %d, время: %02d:00\n", current_day, current_hour);
	fprintf(file, "Инвентарь:\n");

	// Записываем только непустые слоты
	for (int i = 0;


		i < Inventory_size; i++) {
		if (inventory[i] != 0) {  // 0 - это пустой слот
			fprintf(file, "  Слот %d: %s\n", i, item_names[inventory[i]]);
		}
	}

	fprintf(file, "========================\n\n");

	fclose(file);  // Закрываем файл
	printf(" Состояние записано в diary.txt!\n");
}
//ШАБЛОНИЗАТОР ДИАЛОГОВ

void process_letter() {
	// Открываем input.txt для чтения
	FILE* input = fopen("input.txt", "r");
	if (input == NULL) {
		printf(" Ошибка: не удалось открыть input.txt\n");
		return;
	}

	// Открываем output.txt для записи 
	FILE* output = fopen("output.txt", "w");
	if (output == NULL) {
		printf(" Ошибка: не удалось создать output.txt\n");
		fclose(input);
		return;
	}

	printf("--- Результат обработки письма ---\n");

	char line[2048];  // Буфер для одной строки из файла

	// Читаем файл построчно
	while (fgets(line, sizeof(line), input) != NULL) {
		// Здесь мы будем собирать результат
		char result[4096];  // Буфер для результата (побольше, т.к. теги заменяются)
		int r = 0;          // Индекс для записи в result

		// Проходим по каждому символу строки
		for (int i = 0; line[i] != '\0'; i++) {

			// Если встретили '<' - возможно, это тег
			if (line[i] == '<') {

				// Проверяем, не "<NAME>" ли это
				// Сравниваем следующие символы вручную
				if (line[i + 1] == 'N' && line[i + 2] == 'A' &&
					line[i + 3] == 'M' && line[i + 4] == 'E' &&
					line[i + 5] == '>') {
					// Это тег <NAME>. Копируем имя фермера в результат.
					for (int j = 0; farmer_name[j] != '\0'; j++) {
						result[r] = farmer_name[j];
						r++;
					}
					i = i + 5;  // Пропускаем 6 символов тега (цикл ещё +1)
					continue;   // Переходим к следующему символу
				}


				// Проверяем, не "<DAY>" ли это
				else if (line[i + 1] == 'D' && line[i + 2] == 'A' &&
					line[i + 3] == 'Y' && line[i + 4] == '>') {
					// Преобразуем число current_day в строку
					char day_str[10];
					sprintf(day_str, "%d", current_day);

					// Копируем строку с днём в результат
					for (int j = 0; day_str[j] != '\0'; j++) {
						result[r] = day_str[j];
						r++;
					}
					i = i + 4;  // Пропускаем 5 символов тега
					continue;
				}

				// Проверяем, не "<ITEM>" ли это
				else if (line[i + 1] == 'I' && line[i + 2] == 'T' &&
					line[i + 3] == 'E' && line[i + 4] == 'M' &&
					line[i + 5] == '>') {
					// Берём название предмета из нулевого слота
					char* item_str = item_names[inventory[0]];

					// Копируем его в результат
					for (int j = 0; item_str[j] != '\0'; j++) {
						result[r] = item_str[j];
						r++;
					}
					i = i + 5;  // Пропускаем 6 символов тега
					continue;
				}

				// Если это не <NAME>, не <DAY> и не <ITEM> -
				// просто копируем '<' как обычный символ
			}

			// Обычный символ - просто копируем в результат
			result[r] = line[i];
			r++;
		}

		// Завершаем строку нуль-терминатором
		result[r] = '\0';

		// Выводим результат на экран и записываем в файл
		printf("%s", result);
		fprintf(output, "%s", result);
	}

	fclose(input);   
	fclose(output);  

	printf("\n Результат сохранён в output.txt\n");
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