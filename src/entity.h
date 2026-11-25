#ifndef ENITY_H
#define ENITY_H
#include"player.h"

enum EntityType {
	ENT_TREE,ENT_ROCK,ENT_HEAL,ENT_MONSTER
};
struct Entity {
	enum EntityType type; // РЕСУРС, МОНСТР, АПТЕЧКА
	char symbol; // 'T', 'R', 'M', '+'
	int x, y; // Координаты на карте
	int hp, max_hp; // Прочность ресурса или здоровье зверя
	int attack;// Урон (0 для мирных ресурсов)
	int defense; //
	int drop_item_id; // Какой ID предмета из items.txt упадёт в рюкзак
	int is_alive; // 1 -- объект на карте, 0 -- уничтожен/собран
};

struct Entity* generation_entities(int count, int depth);
void free_entities(struct Entity* entities);
int interact_with_entity(struct Player* player, struct Entity* ent, char* event_msg);

#endif