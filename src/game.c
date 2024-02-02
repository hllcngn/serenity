#include "serenity.h"

int game(vect3f hue, Map* map, Player* pl){
attron(COLOR_PAIR(2));
for (int y=0;y<30;y++){
	move((LINES-30)/2+y,(COLS-50)/2);
	for (int x=0;x<50;x++)
		addch(map->map[y][x]);}
getch();
	return 0;}
