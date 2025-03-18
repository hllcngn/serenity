#include "serenity.h"

int run_game(Game* game, Ui* ui, Ref* ref, Player* pl, World* world){
Map *map =world->maplist->next->map, *newmap=map;
char c=0; Instance* inst; do { switch (c){
case K_UP:
case K_DOWN:
case K_LEFT:
case K_RIGHT:	newmap=movement(pl,world,map,c);
		if (newmap!=map){
			if (newmap->type ==OUTDOORS){
				Houselist* hl;
				for (hl =map->houselist;
					hl &&hl->house->id !='a'; hl =hl->next);
				if (hl){
					pl->y =map->houselist->house->y+pl->y;
					pl->x =map->houselist->house->x+pl->x;
				}
			}
			else if (newmap->type ==INDOORS){
				//pl->y =pl->y-map->house->y;
				//pl->x =pl->x-map->house->x;
			}
			map=newmap;
		}
		break;
default:	act(ref, map, pl, c);	break;
}

display(ui, pl, map);

} while((c=getch())!=K_QUIT);	return 0;}



Map* movement(Player* pl, World* world, Map* map, char c){
//Map* newmap =map;
int tp; switch (c){
case K_UP:    if(!check_collision(map, pl->y-1, pl->x)){
			pl->y--;
			int id =check_tp(map, pl->y, pl->x);
			if(id){
				//save_map(map);
				Houselist* hl;
				for (hl =map->houselist;
					hl &&hl->house->id !=id; hl =hl->next);
				if (!hl) break;
				pl->y =pl->y-hl->house->y;
				pl->x =pl->x-hl->house->x;
				Maplist* m =world->maplist;
				for (; m &&strcmp(m->map->name,"House"); m=m->next);
				if (m) map =m->map;
				/*else {
					map =load_map(map->house,map);
					map->previous =NULL;
					map->next =world->maps;
					world->maps->previous =map;
					world->maps =map;
				}*/
			}
		} break;
case K_DOWN:	if(!check_collision(map, pl->y+1, pl->x)){
			pl->y++;
			//if(check_tp(map, pl->y, pl->x))
				//save_map(map);
			//		map =world->maplist->next->map;
			int id =check_tp(map, pl->y, pl->x);
			if(id &&map->type ==INDOORS){
				//save_map(map);
				Maplist* ml;
				for (ml =world->maplist;
					ml &&!strcmp(ml->map->name, "House"); ml =ml->next);
				if (!ml) break;
				pl->y =pl->y+ml->map->houselist->house->y;
				pl->x =pl->x+ml->map->houselist->house->x;
				if (ml) map =ml->map;
			}
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
