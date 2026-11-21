#define _CRT_SECURE_NO_WARNINGS
#include "player.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <time.h>  
#include <conio.h> 

void time_menu(struct GameState* game) {
	int choice = -1;
	while (choice != 0) {
		printf("\n ---УПРАВЛЕНИЕ ВРЕМЕНЕМ---\n");
		printf("[0] Вернуться в меню\n");
		printf("[1] Посмотреть на часы\n");
		printf("[2] Пропустить время\n");
		printf("[3] Лечь спать\n");

		choice = safe_input_int("Выберите действие:");

		switch (choice) {
		case 0:
			printf("Возврат в главное меню...\n");
			break;
		case 1:
			show_time(game->current_day, game->current_hour);

		case 2:
			skip_time(&game->current_day, &game->current_hour);
			break;

		case 3:
			rest_and_sleep(game);
			break;
		default:
			printf("Такого пункта нет!\n ");
			break;
		}
	}


}
void sell_maps(struct GameState* game) {
	if (game->player->map_drawn == 0) {
	printf("\n У вас нет карт для продажи.\n");
	return;
	}
	int total_gold = game->player->map_drawn * 50;
	game->player->gold += total_gold;
	printf("\n Вы продали %d карт и получили %d золото!\n", game->player->map_drawn, total_gold);
	game->player->map_drawn = 0;

}
// функция сна (отдых)
void rest_and_sleep(struct GameState* game) {
	if (game->player == NULL) {
		printf("\n Сначала создайте персонажа!\n");
		return;
	}
	// переводим на утро следующего дня
	game->current_day++;
	game->current_hour = 8;

	// восстанавливаем здоровье и выносливость
	game->player->stamina = game->player->max_stamina;
	game->player->hp = game->player->max_hp;

	printf("\n Вы хорошо отдохнули! Наступил День %d, 08:00.\n", game->current_day);
	printf("Силы и здоровье полностью восстановлены!\n");
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
void search_item_by_name(struct Player* player, char item_names[][MAX_NAME_LEN]) {
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
		if (player->inventory[i] == target_id) {
			printf("%d ", i);  // Печатаем номер слота
			found = 1;         // Запоминаем, что нашли
		}
	}

	if (found == 0) {
		printf("Предмета нет в рюкзаке\n");
	}
	printf("\n");
}

	
struct Player* create_player(const char* name) {
	struct Player* player = (struct Player*)malloc(sizeof(struct Player));
	if (player == NULL) {
		printf("Ошибка!Не хватило памяти для персонажа!\n");
		return NULL;
	}

	strcpy(player->farmer_name, name); //
	player->hp = START_HP;
	player->max_hp = START_HP;
	player->stamina = START_STAMINA;
	player->max_stamina = START_STAMINA;
	player->gold = START_GOLD;
	player->x = 0;
	player->y = 0;
	player->map_drawn = 0;

	for (int i = 0; i < Inventory_size; i++) {
		player->inventory[i] = EMPTY_ITEM;
	}
	return player;

}

struct Player* recreate_player(struct Player* player) {
	if (player != NULL) {
		int answer = safe_input_int("Персонаж уже существует.Удалить?(1-да, 0 - не):\n");
		if (answer != 1) {
			return player;
		}
		destroy_player(player);
	}
char name[MAX_NAME_LEN];
safe_input_string("Введите имя персонажа: ", name, MAX_NAME_LEN);
if (strlen(name) == 0) {
	strcpy(name, "Фермер");       // имя по умолчанию
}

// 3. Создаём нового и отдаём его
return create_player(name);
}
void destroy_player(struct Player* player){
	if (player == NULL) {
		return;
	}
	printf("Память структуры по адресу %p отчищена \n" , (void*)player);
	free(player);
	player = NULL; //обнуляем указатель чтоб избежать исп освобожд памяти в будущем
}

//функция карточки персонажа
void show_player_card(struct GameState* game) {
	if (game->player == NULL) {
		printf("\n Персонаж еще не создан! Выберите пункт [1]\n");
		return;
	}
	printf("\n === КАРТОЧКА ПЕРСОНАЖА ===\n");
	printf("Имя: %s\n", game->player->farmer_name);
	printf("Время: День %d , %02d:00\n", game->current_day, game->current_hour);
	printf("HP: %d/%d\n", game->player->hp, game->player->max_hp);
	printf("Выносливость: %d/%d\n", game->player->stamina, game->player->max_stamina);
	printf("Золото: %d\n", game->player->gold);
	printf("Нарисовано карт: %d\n", game->player->map_drawn);
	printf("---Инвентарь---\n");
	int has_items = 0; //чтоб понять пустой рюкзак или нет
	for (int i = 0; i < Inventory_size; i++) {
		if (game->player->inventory[i] != EMPTY_ITEM) {
			// берем ид предм из игрока и ищем его название в базе gamestate
			printf("Слот %d: %s\n", i, game->item_names[game->player->inventory[i]]);
			has_items = 1;
		}
	}
	if (! has_items) {
		printf("Пусто\n");
	}
	printf("=======================================\n");

}