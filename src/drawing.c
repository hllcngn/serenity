#include "serenity.h"

void display_map(Map* map, vect2i pos){
attron(COLOR_PAIR(CP_BASE));
int y=0;
for (y; pos.y-WGAMEH/2+y<0; y++){
	move(WGAMEY+y,WGAMEX);
	for (int x=0;x<WGAMEW;x++) addch(' ');}
for (y; pos.y-WGAMEH/2+y<map->h && y<WGAMEH; y++){
	move(WGAMEY+y,WGAMEX);
	int x=0;
	for (x; pos.x-WGAMEW/2+x<0; x++) addch(' ');
	for (x; pos.x-WGAMEW/2+x<map->w && x<WGAMEW; x++)
		if	(map->fg[pos.y-WGAMEH/2+y][pos.x-WGAMEW/2+x])
			addch(map->fg[pos.y-WGAMEH/2+y][pos.x-WGAMEW/2+x]);
		else if	(map->clsn[pos.y-WGAMEH/2+y][pos.x-WGAMEW/2+x])
			addch(map->clsn[pos.y-WGAMEH/2+y][pos.x-WGAMEW/2+x]);
		else	addch(map->bg[pos.y-WGAMEH/2+y][pos.x-WGAMEW/2+x]);
	for (x; x<WGAMEW; x++) addch(' ');}
for (y; y<WGAMEH; y++){
	move(WGAMEY+y,WGAMEX);
	for (int x=0;x<WGAMEW;x++) addch(' ');}

for (Instance* it=map->inst;it;it=it->next)
if     (it->y+it->h>pos.y-WGAMEH/2 &&it->y<pos.y+WGAMEH/2
      &&it->x+it->w>pos.x-WGAMEW/2 &&it->x<pos.x+WGAMEW/2){
int yy =it->y<pos.y-WGAMEH/2? pos.y-WGAMEH/2-it->y :0;
for (yy;yy<it->h &&it->y+yy<pos.y+WGAMEH/2;yy++){
int xx =it->x<pos.x-WGAMEW/2? pos.x-WGAMEW/2-it->x :0;
for (xx;xx<it->w &&it->x+xx<pos.x+WGAMEW/2;xx++){
	move(   WGAMEY+WGAMEH-((pos.y+WGAMEH/2)-it->y)+yy,
		WGAMEX+WGAMEW-((pos.x+WGAMEW/2)-it->x)+xx);
	if (it->map[yy][xx]!=' ') addch(it->map[yy][xx]);}}}
return;}


void display_pl(Player* pl, Map* map){
if (map->fg[pl->y][pl->x]) return;
else if (map->it[pl->y][pl->x]){
Instance* it; for (it=map->inst;
it &&it->id!=map->it[pl->y][pl->x];
it=it->next);
if(it->info[pl->y-it->y][pl->x-it->x]=='f') return;}
move(WGAMEY+WGAMEH/2,WGAMEX+WGAMEW/2); //TODO exact coordinates
attron(COLOR_PAIR(CP_NORMAL));
addch(' ');	return;}
