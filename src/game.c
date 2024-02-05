#include "serenity.h"

int game(vect3f hue, Map* map, Player* pl){
char c=0; do {
switch (c){	case K_UP:
		case K_DOWN:
		case K_LEFT:
		case K_RIGHT:	movement(c,pl,map);	break;
		default:				break;}
display_map(map, (vect2i){pl->y,pl->x});
display_pl(pl, map);
} while((c=getch())!=K_QUIT);
	return 0;}




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
default:	break;}
return;}

int check_collision(vect2i pos, Map* map){
if (pos.y>=0	  &&pos.x>=0
  &&pos.y<map->h  &&pos.x<map->w
  &&!map->clsn[pos.y][pos.x]){
	if (map->it[pos.y][pos.x]){
		Interactive* it; for (it=map->inter;
		it &&it->id!=map->it[pos.y][pos.x];
		it=it->next);
		if (it->info[pos.y-it->y][pos.x-it->x]=='X'){
			return 1;}
		else return 0;}
	else return 0;}
else return 1;}
