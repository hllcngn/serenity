#include "serenity.h"

int game(vect3f hue, Map* map, Player* pl){
char c=0; do {
display_map(map, (vect2i){pl->y,pl->x});
display_pl(pl);
switch (c){	case K_UP:
		case K_DOWN:
		case K_LEFT:
		case K_RIGHT:	movement(c,pl,map);	break;
		default:				break;}
} while((c=getch())!=K_QUIT);
	return 0;}

void movement(char c, Player* pl, Map* map){
return;}
