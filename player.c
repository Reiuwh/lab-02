#include "player.h"
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