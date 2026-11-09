#include "map.h"
#include <stdlib.h>
#include <stdio.h>


// Функция для освобождения памяти (чтобы не было утечек)
void free_map(char** map, int height) {
	if (map == NULL) return;
	for (int i = 0; i < height; i++) {
		free(map[i]); // освобождаем каждую строку
	}
	free(map); // освобождаем сам массив указателей
}

// генерация нового этажа
char** generate_dungeon(int* height, int* width, int* hero_x, int* hero_y, int* stairs_down_x, int* stairs_down_y, int* stairs_up_x, int* stairs_up_y) {
	// Случайный размер от 5 до 30 
	*height = rand() % (MAX_MAP_SIZE - MIN_MAP_SIZE + 1) + MIN_MAP_SIZE;
	*width = rand() % (MAX_MAP_SIZE - MIN_MAP_SIZE + 1) + MIN_MAP_SIZE;

	// выделяем память под карту (указатель на указатель)
	char** map = (char**)malloc(*height * sizeof(char*));//выделяет место под список адресов строк
	for (int i = 0; i < *height; i++) {
		map[i] = (char*)malloc(*width * sizeof(char)); //выделяет место под каждую строку
	}

	// заполняем карту
	for (int y = 0; y < *height; y++) {
		for (int x = 0; x < *width; x++) {
			// Границы - стены
			if (y == 0 || y == *height - 1 || x == 0 || x == *width - 1) {
				map[y][x] = '#';
			}
			else {
				int r = rand() % 100;
				if (r < 10) map[y][x] = 'T';       // 10% деревья
				else if (r < 20) map[y][x] = 'R';  // 10% камни
				else if (r < 30) map[y][x] = '~';  // 10% лед
				else map[y][x] = '.';              //  пол
			}
		}
	}

	// ставим героя в случайное пустое место (не на границе)
	*hero_x = rand() % (*width - 2) + 1;
	*hero_y = rand() % (*height - 2) + 1;

	// лестница вверх (выход) появляется прямо под героем
	*stairs_up_x = *hero_x;
	*stairs_up_y = *hero_y;
	map[*hero_y][*hero_x] = 'U'; // Сначала ставим лестницу

	// ставим лестницу вниз (D) в другое случайное место
	do {
		*stairs_down_x = rand() % (*width - 2) + 1;
		*stairs_down_y = rand() % (*height - 2) + 1;
	} while (*stairs_down_x == *hero_x && *stairs_down_y == *hero_y);
	map[*stairs_down_y][*stairs_down_x] = 'D';

	return map; // возвращаем готовую карту
}
// отрисовка карты на экране
void print_map(char** map, int height, int width, int depth) {
	system("cls"); // Очищаем экран чтоб карта рисовалась поверх предыдущей
	printf("=== ПОДЗЕМЕЛЬЕ. Глубина: %d ===\n", depth);
	printf("Управление: W (вверх), A (влево),S(вниз), D(вправо), Q(выход домой с лестницы U)\n\n");
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			printf("%c ", map[y][x]);
		}
		printf("\n");
	}
}

// проверка: нельзя ли встать на клетку (x, y)
// возвращает 1, если клетка закрыта (стена, дерево, камень или за краем карты)
int is_blocked(char** map, int height, int width, int x, int y) {
	// сначала проверяем границы, иначе обратимся к памяти за пределами карты
	if (x < 0 || x >= width || y < 0 || y >= height) return 1;
	if (map[y][x] == '#' || map[y][x] == 'T' || map[y][x] == 'R') return 1;
	return 0;
}

void start_adventure() {
	int height, width;
	int hero_x, hero_y;
	int stairs_down_x, stairs_down_y;
	int stairs_up_x, stairs_up_y;
	int depth = 1;

	char** map = generate_dungeon(&height, &width, &hero_x, &hero_y,
		&stairs_down_x, &stairs_down_y,
		&stairs_up_x, &stairs_up_y);

	// Запоминаем, что лежит под героем, и ставим героя на карту
	char under = map[hero_y][hero_x];
	map[hero_y][hero_x] = '@';

	int game_over = 0;

	while (!game_over) {
		print_map(map, height, width, depth);

		char key = _getch();
		if (key >= 'a' && key <= 'z') key -= 32; //превращает маленькую букву в большую (разница в таблице символов 32), чтобы работали и w, и W

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

		int new_x = hero_x;
		int new_y = hero_y;
		int dx = 0, dy = 0;

		if (key == 'W') { new_y--; dy = -1; }
		else if (key == 'S') { new_y++; dy = 1; }
		else if (key == 'A') { new_x--; dx = -1; }
		else if (key == 'D') { new_x++; dx = 1; }
		else continue;

		// если идти некуда, просто ждём следующую клавишу
		if (is_blocked(map, height, width, new_x, new_y)) continue;

		// возвращаем на старое место то, что там было
		map[hero_y][hero_x] = under;

		// Переходим на новую клетку
		hero_x = new_x;
		hero_y = new_y;


		// Если встали на лёд, скользим в том же направлении
		while (map[hero_y][hero_x] == '~') {
			int next_x = hero_x + dx;
			int next_y = hero_y + dy;

			// Впереди препятствие: останавливаемся на льду
			if (is_blocked(map, height, width, next_x, next_y)) break;

			// Иначе делаем ещё один шаг
			hero_x = next_x;
			hero_y = next_y;
		}

		// запоминаем, что на новой клетке, и ставим туда героя
		under = map[hero_y][hero_x];
		map[hero_y][hero_x] = '@';

		// Дошли до лестницы вниз: строим новый этаж
		if (under == 'D') {
			free_map(map, height);   // старую карту больше не нужно хранить
			depth++;

			map = generate_dungeon(&height, &width, &hero_x, &hero_y,
				&stairs_down_x, &stairs_down_y,
				&stairs_up_x, &stairs_up_y);

			// cнова запоминаем, что под героем на новом этаже, и ставим его
			under = map[hero_y][hero_x];
			map[hero_y][hero_x] = '@';

			printf("\n Вы спустились на глубину %d!\n", depth);
			_getch();
			continue;
		}

		map[hero_y][hero_x] = '@';
	}

	free_map(map, height);
}