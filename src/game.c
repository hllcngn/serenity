#include "serenity.h"

int game(vect3f hue, Map* map, Player* pl, Info *info){
char c=0; Instance* inst; do { switch (c){
case K_UP:
case K_DOWN:
case K_LEFT:
case K_RIGHT:	movement(c,pl,map);	break;
default:
	//Instance* inst =check_inst((vect2i){pl->y,pl->x},map);
	inst =check_inst((vect2i){pl->y,pl->x},map);
	if (inst &&inst->inter->inter[pl->y-inst->y][pl->x-inst->x]=='i'){
		Actionlist* al;
		 for (al=inst->inter->actionlist;
		   al &&al->action->k!=c;
		   al=al->next);
		 if (al) al->action->action(inst,map,info);}	break;}

display_map(map, (vect2i){pl->y,pl->x});
display_pl(pl, map);
display_notice((vect2i){pl->y,pl->x},map);
} while((c=getch())!=K_QUIT);	return 0;}



void movement(char c, Player* pl, Map* map){
switch (c){
case K_UP:    if(!check_collision((vect2i){pl->y-1,pl->x}, map))
			pl->y--;	break;
case K_DOWN:  if(!check_collision((vect2i){pl->y+1,pl->x}, map))
			pl->y++;	break;
case K_LEFT:  if(!check_collision((vect2i){pl->y,pl->x-1}, map))
			pl->x--;	break;
case K_RIGHT: if(!check_collision((vect2i){pl->y,pl->x+1}, map))
			pl->x++;	break;
default:				break;}}

int check_collision(vect2i pos, Map* map){
if (pos.y>=0	  &&pos.x>=0
  &&pos.y<map->h  &&pos.x<map->w
  &&!map->clsn[pos.y][pos.x]){
	Instance* it =check_inst(pos,map);
	if (it &&it->inter->info[pos.y-it->y][pos.x-it->x]=='X')
		return 1;
	return 0;}
return 1;}
