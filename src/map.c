#include "map.h"
#include"player.h"
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>


// Функция для освобождения памяти (чтобы не было утечек)
void free_map(struct Map* map) {
	if (map == NULL) return;
	for (int i = 0; i < map->height; i++) {
		free(map->field[i]); // освобождаем каждую строку
	}
	free(map->field);
	free(map);// освобождаем сам массив указателей
}

// генерация нового этажа
struct Map* generate_dungeon(int depth) {
	// выделяем память под карту (указатель на
	struct Map* map = (struct Map*)malloc(sizeof(struct Map));//выделяет место под список адресов строк
	if (map == NULL) return NULL;

	map->depth = depth;
	map->biome = rand() % 4; //случ биом

	// Случайный размер от 5 до 30 
	map->height = rand() % (MAX_MAP_SIZE - MIN_MAP_SIZE + 1) + MIN_MAP_SIZE;
	map->width = rand() % (MAX_MAP_SIZE - MIN_MAP_SIZE + 1) + MIN_MAP_SIZE;

	
	// выделяем память под карту (указатель на указатель)
	map->field = (char**)malloc(map->height * sizeof(char*));//выделяет место под список адресов строк
	for (int i = 0; i < map->height; i++) {
		map->field[i] = (char*)malloc(map->width * sizeof(char)); //выделяет место под каждую строку
	}

	// заполняем карту
	for (int y = 0; y < map->height; y++) {
		for (int x = 0; x < map->width; x++) {
			// Границы - стены
			if (y == 0 || y == map->height - 1 || x == 0 || x == map->width - 1) {
				map->field[y][x] = '#';
			}
			else {
				int r = rand() % 100;
				if (r < 10) map->field[y][x] = 'T';       // 10% деревья
				else if (r < 20) map->field[y][x] = 'R';  // 10% камни
				else if (r < 30) map->field[y][x] = '~';  // 10% лед
				else map->field[y][x] = '.';              //  пол
			}
		}
	}

	// ставим героя в случайное пустое место (не на границе)
	map->hero_x = rand() % (map->width - 2) + 1;
	map->hero_y = rand() % (map->height - 2) + 1;

	// лестница вверх (выход) появляется прямо под героем
	map->stairs_up_x = map->hero_x;
	map->stairs_up_y = map->hero_y;
	map->field[map->hero_y][map->hero_x] = 'U'; // Сначала ставим лестницу

	// ставим лестницу вниз (D) в другое случайное место
	do {
		map->stairs_down_x = rand() % (map->width - 2) + 1;
		map->stairs_down_y = rand() % (map->height - 2) + 1;
	} while (map->stairs_down_x == map->hero_x && map->stairs_down_y == map->hero_y);
	map->field[map->stairs_down_y][map->stairs_down_x] = 'D';

	return map; // возвращаем готовую карту
}
// отрисовка карты на экране
void print_map(struct Map* map, struct Player* player) {
	system("cls"); // Очищаем экран чтоб карта рисовалась поверх предыдущей

	if (player!= NULL) {
		printf("HP: %d/%d | Выносливость: %d/%d | Золот: %d | Время: %02d:00\n",
			player->hp, player->max_hp, player->stamina, player->max_stamina, player->gold, current_hour);
	}
	printf("=== ПОДЗЕМЕЛЬЕ. Глубина: %d ===\n", map->depth);
	if (map->biome == BIOME_FOREST) printf("Биом: Редколесье\n");
	else if (map->biome == BIOME_SWAMP) printf("Биом: Топь (стамина -2)\n");
	else if (map->biome == BIOME_RUINS) printf("Биом: Каменные руины\n");
	else printf("Биом: Кристальная пещера (золотп х3)\n");

	printf("Управление: W/A/S/D -ходьба, M - зарисовать карту, Q(выход домой с лестницы U)\n\n");

	for (int y = 0; y < map->height; y++) {
		for (int x = 0; x < map->width; x++) {
			printf("%c ", map->field[y][x]);
		}
		printf("\n");
	}
}

// проверка: нельзя ли встать на клетку (x, y)
// возвращает 1, если клетка закрыта (стена, дерево, камень или за краем карты)
int is_blocked(struct Map* map, int x, int y) {
	// сначала проверяем границы, иначе обратимся к памяти за пределами карты
	if (x < 0 || x >= map->width || y < 0 || y >= map->height) return 1;
	if (map->field[y][x] == '#' || map->field[y][x] == 'T' || map->field[y][x] == 'R') return 1;
	return 0;
}

void start_adventure(struct Player* player) {
	if (player == NULL) return;

	int depth = 1;

	struct Map* map = generate_dungeon(depth);
	// Запоминаем, что лежит под героем, и ставим героя на карту
	char under = map->field[map->hero_y][map->hero_x];
	map->field[map->hero_y][map->hero_x] = '@';

	int game_over = 0;
	int is_mapped = 0;//нарисована ли карта на этом этаже

	while (!game_over) {
		print_map(map,player);

		char key = _getch();
		if (key >= 'a' && key <= 'z') key -= 32; //превращает маленькую букву в большую (разница в таблице символов 32), чтобы работали и w, и W

		if (key == 'M') {
			if (is_mapped) {
				printf("\n Вы уже зарисовали карту этого этажа!(нажмите любую клавишу)\n");
			}
			else if (player->stamina < 5) {
				printf("\n Не хватает выносливости!Нужно 5!!(нажмите любую клавишу)\n");
			}
			else {
				player->stamina -= 5;
				player->map_drawn++;
				is_mapped = 1;
				printf("\n Вы потратили 5 стамины и зарисовали карту!(Всего карт: %d)(нажмите любую клавишу)\n", player->map_drawn);
			}
			_getch();
			continue;
		}
		if (key == 'Q') {
			if (under == 'U') {   // выйти можно, только стоя на лестнице вверх
				printf("\n Вы благополучно выбрались из подземелья!\n");
				system("pause");
				game_over = 1;
			}
			else {
				printf("\n Вы не на лестнице вверх! Нельзя выйти.\n");
				_getch();
			}
			continue;
		}

		int new_x = map->hero_x;
		int new_y = map->hero_y;
		int dx = 0, dy = 0;

		if (key == 'W') { new_y--; dy = -1; }
		else if (key == 'S') { new_y++; dy = 1; }
		else if (key == 'A') { new_x--; dx = -1; }
		else if (key == 'D') { new_x++; dx = 1; }
		else continue;

		// если идти некуда, просто ждём следующую клавишу
		if (is_blocked(map, new_x, new_y)) continue;

		//расход стамины за шаг
		int stamina_cost = 1;
		if (map->biome == BIOME_SWAMP) {
			stamina_cost = 2; // в топи шаг -2
		}
		player->stamina -= stamina_cost;


		// возвращаем на старое место то, что там было
		map->field[map->hero_y][map->hero_x] = under;

		// Переходим на новую клетку
		map->hero_x = new_x;
		map->hero_y = new_y;


		// Если встали на лёд, скользим в том же направлении
		while (map->field[map->hero_y][map->hero_x] == '~') {
			int next_x = map->hero_x + dx;
			int next_y = map->hero_y + dy;

			// Впереди препятствие: останавливаемся на льду
			if (is_blocked(map,next_x, next_y)) break;

			// Иначе делаем ещё один шаг
			map->hero_x = next_x;
			map->hero_y = next_y;
		}

		// запоминаем, что на новой клетке, и ставим туда героя
		under = map->field[map->hero_y][map->hero_x];
		map->field[map->hero_y][map->hero_x] = '@';

		// Дошли до лестницы вниз: строим новый этаж
		if (under == 'D') {
			player->stamina -= 5;
			current_hour++;
			if (current_hour >= 24) { current_hour = 0;current_day++; }

			int reward = 10 * map->depth;
			if (map->biome == BIOME_CRYSTAL) reward *= 3;
			player->gold += reward;

			printf("\n Вы спустились на глубину %d!Получено %d золота. (нажмите клавишу)\n", map->depth + 1,reward);

			free_map(map);   // старую карту больше не нужно хранить
			depth++;
			map = generate_dungeon(depth);

			// cнова запоминаем, что под героем на новом этаже, и ставим его
			under = map->field[map->hero_y][map->hero_x];
			map->field[map->hero_y][map->hero_x] = '@';
			is_mapped = 0; //можно рисовать карту снова
			printf("\n Вы спустились на глубину %d!\n", depth);
			_getch();
			continue;
		}
		if (player->stamina <= 0 || current_hour == 2) {
			printf("\n========================================================\n");
			printf("Вы упали без сил! Спасательная служба доставила вас домой.\n");
			printf("За услуги списана половина заработанного золота.\n");
			printf("==========================================================\n");
			player->gold /= 2;
			player->stamina = 0;
			game_over = 1;
			_getch();
		}

		map->field[map->hero_y][map->hero_x] = '@';
	}

	free_map(map);
}

