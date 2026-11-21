#define _CRT_SECURE_NO_WARNINGS
#include "utils.h"
#include "player.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <time.h>    // для rand() и srand()
#include <conio.h> // для _getch() (движение без Enter)




int main() {
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	struct GameState game;
	game.player = NULL;
	game.current_day = 1;
	game.current_hour = 8;
	
	srand(time(NULL)); // инициализация случайных чисел (для карты)
	load_items("items.txt",game.item_names);





	int choice = -1;
	while (1) {
		printf("\n === МЕНЮ ФЕРМЕРА ===\n");

		if (game.player != NULL) {
			printf("Привет, %s! \n ", game.player->farmer_name);
		}
		else {
			printf("Привет, путешественник!(персонаж пока не создан)\n");
		}
		
		printf("[0] Выход\n"
			"[1] Создать /пересоздать персонажа\n"
			"[2] Карточка персонажа\n"
			"[3] Посмотреть инвентарь\n"
			"[4] Положить предмет в слот\n"
			"[5] Выбросить предмет\n"
			"[6] Отчистка от мусора\n"
			"[7] Поиск предмета по названию\n"
			"[8] Записать состояние в дневник\n"
			"[9] Обработать письмо NPC\n"
			"[10] Отправиться в приключение (Подземелье)\n"
			"[11] Продать карты\n"// новый пункт
			"[12] Посмотреть на часы\n"
			"[13] Пропустить время\n"
			"[14] Лечь спать(отдых)\n");

		choice = safe_input_int("Выберите пункт:");
		
		if (choice >= 3 && choice <= 11 && game.player == NULL) {
			printf("\n Сначала создайте персонажа(пункт 1)!\n");
			continue;
		}

		switch (choice) {
		case 0:
			printf("Выход из игры.До скорой встречи!\n");
			if (game.player != NULL) destroy_player(game.player);
			return 0;
		case 1:
			game.player = recreate_player(game.player);
			break;
		case 2:
			show_player_card(&game);
			break;
			
		case 3:
			show_inventory(game.player->inventory, game.item_names);
			break;

		case 4:
			put_item(game.player->inventory);
			break;

		case 5:
			discard_item(game.player->inventory);
			break;

		case 6:
			clean_trash(game.player->inventory);
			break;

		case 7:
			search_item_by_name(game.player, game.item_names);
			break;

		case 8:
			write_diary(&game);
			break;

		case 9:
			process_letter(&game);
			break;

		case 10: start_adventure(&game); 
			break;

		case 11:
			sell_maps(&game);
			break;

		case 12:
			show_time(game.current_day, game.current_hour);

		case 13:
			skip_time(&game.current_day, &game.current_hour);
			break;

		case 14:
			rest_and_sleep(&game);
			break;

		default:
			printf(" Такого пункта нет. Попробуйте снова.\n");
			break;
		}
	
	}
	return 0;
}