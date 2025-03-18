#include "serenity.h"

int run_game(Game* game, Ui* ui, Ref* ref, Player* pl, World* world){
Map *map =world->maps, *newmap=map;
char c=0; Instance* inst; do { switch (c){
case K_UP:
case K_DOWN:
case K_LEFT:
case K_RIGHT:	newmap=movement(pl,world,map,c);
		if (newmap!=map){
			if (newmap->type ==OUTDOORS){
				pl->y =newmap->house->y+pl->y;
				pl->x =newmap->house->x+pl->x;
			}
			else if (newmap->type ==INDOORS){
				//pl->y =pl->y-map->house->y;
				//pl->x =pl->x-map->house->x;
			}
			map=newmap;
		} break;
default:	act(ref, map, pl, c);	break;
}

display(ui, pl, map);

} while((c=getch())!=K_QUIT);	return 0;}



Map* movement(Player* pl, World* world, Map* map, char c){
//Map* newmap =map;
int tp; switch (c){
case K_UP:    if(!check_collision(map, pl->y-1, pl->x)){
			pl->y--;
			if(check_tp(map, pl->y, pl->x)){
				//save_map(map);
				pl->y =pl->y-map->house->y;
				pl->x =pl->x-map->house->x;
				Map* m =world->maps;
				for (; m &&strcmp(m->name,"House"); m=m->next);
				if (m) map =m;
				else {
					map =load_map(map->house,map);
					map->previous =NULL;
					map->next =world->maps;
					world->maps->previous =map;
					world->maps =map;
				}
			}
		} break;
case K_DOWN:	if(!check_collision(map, pl->y+1, pl->x)){
			pl->y++;
			if(check_tp(map, pl->y, pl->x))
				//save_map(map);
					map =world->maps->next;
		} break;
case K_LEFT:	if(!check_collision(map, pl->y, pl->x-1))
			pl->x--;	break;
case K_RIGHT:	if(!check_collision(map, pl->y, pl->x+1))
			pl->x++;	break;
default:				break;}
return map;}


int check_collision(Map* map, int y, int x){
if (y<0 ||x<0 ||y>=map->h ||x>=map->w)	return 1;
if (map->clsn[y][x])			return 1;
Instance* it =get_inst(map, y, x);
if (it &&it->inter->info[y-it->y][x-it->x]=='X') return 1;
return 0;}

int check_tp(Map* map, int y, int x){
if (y>=0 &&x>=0 &&y<map->h &&x<map->w)
	return map->tp[y][x];
return 0;}
