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
case K_UP:    if (pl->y>0     &&!map->clsn[pl->y-1][pl->x])
	if (map->it[pl->y-1][pl->x]){
		Interactive* it;
		for (it=map->inter;
		it &&it->id!=map->it[pl->y-1][pl->x];
		it=it->next);
		if (it->info[pl->y-1-it->y][pl->x-it->x]=='X') break;}
	pl->y--;	break;
case K_DOWN:  if (pl->y<map->h-1 &&!map->clsn[pl->y+1][pl->x]) pl->y++;	break;
case K_LEFT:  if (pl->x>0	     &&!map->clsn[pl->y][pl->x-1]) pl->x--;	break;
case K_RIGHT: if (pl->x<map->w-1 &&!map->clsn[pl->y][pl->x+1]) pl->x++;	break;
default:	break;}
return;}
