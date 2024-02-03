#include "serenity.h"

int game(vect3f hue, Map* map, Player* pl){
char c=0; do {
switch (c){	case K_UP:
		case K_DOWN:
		case K_LEFT:
		case K_RIGHT:	movement(c,pl,map);	break;
		default:				break;}
display_map(map, (vect2i){pl->y,pl->x});
display_pl(pl);
} while((c=getch())!=K_QUIT);
	return 0;}

void movement(char c, Player* pl, Map* map){
if	(c==K_UP    && pl->y>0)		pl->y--;
else if	(c==K_DOWN  && pl->y<map->h-1)	pl->y++;
else if	(c==K_LEFT  && pl->x>0)		pl->x--;
else if	(c==K_RIGHT && pl->x<map->w-1)	pl->x++;
return;}
