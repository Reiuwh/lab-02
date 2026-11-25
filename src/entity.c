#include"entity.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

struct Entity* generate_entities(int count, int depth) {
	struct Entity* entities = (struct Entity*)malloc(count * sizeof(struct Entity));
	if (entities == NULL) return NULL;

	for (int i = 0; i < count; i++) {
		entities[i].is_alive = 1;
		int r = rand() % 100;

		if (r < 40) {
			entities[i].type = ENT_TREE;
			entities[i].symbol = 'T';
			entities[i].hp = entities[i].max_hp = 10 + (depth * 5);
			entities[i].attack = 0;
			entities[i].drop_item_id = 1;

		}
		else if (r < 70) {
			entities[i].type = ENT_ROCK;
			entities[i].symbol = 'R';
			entities[i].hp = entities[i].max_hp = 15 + (depth * 5);
			entities[i].attack = 0;
			entities[i].drop_item_id = 2;
		}
		else if (r < 85) {
			entities[i].type = ENT_HEAL;
			entities[i].symbol = '+';
			entities[i].hp = 1;
			entities[i].attack = 0;
			entities[i].defense = 0;
			entities[i].drop_item_id = -1;
		}
		else {
			entities[i].type = ENT_MONSTER;
			entities[i].symbol = 'M';
			entities[i].hp = entities[i].max_hp = 20 + (depth * 10);
			entities[i].attack = 5 + (depth * 2);
			entities[i].defense = 2 + depth; //защита монстра растет с этажом
			entities[i].drop_item_id = 5; //дропает золото
		}
	}
	return entities;
}

void free_entities(struct Entity* entities) {
	if (entities != NULL) {
		printf("Память массива сущностей по адресу %p освобождена\n", (void*)entities);
		free(entities);
	}
}
int interact_with_entity(struct Player* player, struct Entity* ent, char* event_msg) {
	if (ent == NULL || !ent->is_alive) return 1;

	if (ent->type == ENT_TREE || ent->type == ENT_ROCK) {
		ent->hp -= player->attack;
		sprintf(event_msg, "Вы ударили объект.Осталось прочности: %d", ent->hp > 0 ? ent->hp : 0);

		if (ent->hp <= 0) {
			ent->is_alive = 0;
			//
			int placed = 0;
			for (int i = 0; i < Inventory_size; i++) {
				if (player->inventory[i] == EMPTY_ITEM) {
					player->inventory[i] = ent->drop_item_id;
					sprintf(event_msg, "Вы разрушили объект и получили предмет (%d)!", ent->drop_item_id);
					placed = 1;
					break;
				}
			}
			if (!placed)sprintf(event_msg, "Рюкзак полон!Ресурс потерян((");
		}
		return 0;
	}
	if (ent->type == ENT_HEAL) {
		ent->is_alive = 0;
		player->hp += 20;
		if (player->hp > player->max_hp) player->hp = player->max_hp;
		sprintf(event_msg, "Вы подобрали аптечку и восстановили здоровье!");
		return 1;
	}

		if (ent->type == ENT_MONSTER) {
			int player_dmg = player->attack - (ent->defense / 2);
			if (player_dmg < 1)player_dmg = 1;
			ent->hp -= player_dmg;

			sprintf(event_msg, "Вы ударили понстра на %d!", player_dmg);

			//
			if (ent->hp <= 0) {
				ent->is_alive = 0; //
				player->gold += 15;
				sprintf(event_msg, "Вы победили монстра монстра!Получено золото.");
				return 1; //
			}
			else {
				int mon_dmg = ent->attack - (player->defense / 2); //
				if (mon_dmg < 1) mon_dmg = 1;
				player->hp -= mon_dmg; //

				char temp[100];
				sprintf(temp, "Монстр ударил вас на %d!", mon_dmg);
				strcat(event_msg, temp);
			}
			return 0; //
		}
		return 0;
	}