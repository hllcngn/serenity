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
if     (it->y+it->inter->h>pos.y-WGAMEH/2 &&it->y<pos.y+WGAMEH/2
      &&it->x+it->inter->w>pos.x-WGAMEW/2 &&it->x<pos.x+WGAMEW/2){
int yy =it->y<pos.y-WGAMEH/2? pos.y-WGAMEH/2-it->y :0;
for (yy;yy<it->inter->h &&it->y+yy<pos.y+WGAMEH/2;yy++){
int xx =it->x<pos.x-WGAMEW/2? pos.x-WGAMEW/2-it->x :0;
for (xx;xx<it->inter->w &&it->x+xx<pos.x+WGAMEW/2;xx++){
	move(   WGAMEY+WGAMEH-((pos.y+WGAMEH/2)-it->y)+yy,
		WGAMEX+WGAMEW-((pos.x+WGAMEW/2)-it->x)+xx);
	if (it->inter->map[yy][xx]!=' ') addch(it->inter->map[yy][xx]);}}}
return;}


void display_pl(Player* pl, Map* map){
if (map->fg[pl->y][pl->x]) return;
Instance* it =check_inst((vect2i){pl->y,pl->x},map);
if(it &&it->inter->info[pl->y-it->y][pl->x-it->x]=='f') return;
move(WGAMEY+WGAMEH/2,WGAMEX+WGAMEW/2); //TODO exact coordinates
attron(COLOR_PAIR(CP_NORMAL));
addch(' ');	return;}


void display_notice(vect2i pos, Map* map){
Instance* inst =check_inst(pos,map);
if (inst &&inst->inter->inter[pos.y-inst->y][pos.x-inst->x]=='i'){
	attron(COLOR_PAIR(CP_BASE));
	move(WGAMEY+WGAMEH/2+1,WGAMEX+WGAMEW/2+2);
	for (int i=0;i<inst->inter->action->labellen;i++) addch('-');
	move(WGAMEY+WGAMEH/2+2,WGAMEX+WGAMEW/2+1);
	addch('|'); printw("%s",inst->inter->action->label);
	addch('|'); attron(COLOR_PAIR(CP_BASE));
	move(WGAMEY+WGAMEH/2+2,WGAMEX+WGAMEW/2+2);
	attron(A_UNDERLINE);
	addch(inst->inter->action->label[0]); attroff(A_UNDERLINE);
	move(WGAMEY+WGAMEH/2+3,WGAMEX+WGAMEW/2+2);
	for (int i=0;i<inst->inter->action->labellen;i++) addch('-');}
return;}
