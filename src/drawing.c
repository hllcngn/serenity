#include "serenity.h"

void display_map(Map* map, vect2i pos){
attron(COLOR_PAIR(CP_BASE));
int y=0; for (y; pos.y-WGAMEH/2+y<0; y++){
	move(WGAMEY+y,WGAMEX);
	for (int x=0;x<WGAMEW;x++)
		addch(' ');}
for (y; pos.y-WGAMEH/2+y<map->h && y<WGAMEH; y++){
	move(WGAMEY+y,WGAMEX);
	int x=0; for (x; pos.x-WGAMEW/2+x<0; x++)
		addch(' ');
	for (x; pos.x-WGAMEW/2+x<map->w && x<WGAMEW; x++)
		addch(map->map[pos.y-WGAMEH/2+y][pos.x-WGAMEW/2+x]);
	for (x; x<WGAMEW; x++)
		addch(' ');}
for (y; y<WGAMEH; y++){
	move(WGAMEY+y,WGAMEX);
	for (int x=0;x<WGAMEW;x++)
		addch(' ');}
	return;}

void display_pl(Player* pl){
move(WGAMEY+WGAMEH/2,WGAMEX+WGAMEW/2);
attron(COLOR_PAIR(CP_NORMAL));
addch(' ');	return;}
