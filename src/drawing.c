#include "serenity.h"

void display_map(Map* map, vect2i pos){
vect2i wgame =(vect2i){(LINES-WGAMEH)/2,(COLS-WGAMEW)/2};
vect2i cam   =(vect2i){pos.y-WGAMEH/2,  pos.x-WGAMEW/2};
int y=0; attron(COLOR_PAIR(CP_BASE));
for (y; cam.y+y<0; y++){
	move(wgame.y+y,wgame.x);
	for (int x=0;x<WGAMEW;x++) addch(' ');}
for (y; cam.y+y<map->h &&y<WGAMEH; y++){
	int x=0; move(wgame.y+y,wgame.x);
	for (x; cam.x+x<0; x++)    addch(' ');
	for (x; cam.x+x<map->w &&x<WGAMEW; x++)
		if	(map->fg[cam.y+y][cam.x+x])
			addch(map->fg[cam.y+y][cam.x+x]);
		else if	(map->clsn[cam.y+y][cam.x+x])
			addch(map->clsn[cam.y+y][cam.x+x]);
		else	addch(map->bg[cam.y+y][cam.x+x]);
	for (x; x<WGAMEW; x++)     addch(' ');}
for (y; y<WGAMEH; y++){
	move(wgame.y+y,wgame.x);
	for (int x=0;x<WGAMEW;x++) addch(' ');}

for (Instance* in=map->inst;in;in=in->next)
if     (in->y+in->inter->h>cam.y &&in->y<cam.y+WGAMEH
      &&in->x+in->inter->w>cam.x &&in->x<cam.x+WGAMEW){
int yy =in->y<cam.y? cam.y-in->y :0;
for (yy; yy<in->inter->h &&in->y+yy<cam.y+WGAMEH; yy++){
	int xx =in->x<cam.x? cam.x-in->x :0;
	for (xx; xx<in->inter->w &&in->x+xx<cam.x+WGAMEW; xx++){
		move(   wgame.y+WGAMEH-(cam.y+WGAMEH-in->y)+yy,
			wgame.x+WGAMEW-(cam.x+WGAMEW-in->x)+xx);
		if (in->inter->map[yy][xx]!=' ')
			addch(in->inter->map[yy][xx]);}}}
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
	for (int i=0;i<inst->inter->actionlist->action->labellen;i++)
		addch('-');
	int i=0; Actionlist *al=inst->inter->actionlist, *al2;
	for (al;al;al=al->next){
		move(WGAMEY+WGAMEH/2+2+i,WGAMEX+WGAMEW/2+1);
		addch('|');printw("%s",al->action->label);addch('|'); 
		attron(A_UNDERLINE);
		move(WGAMEY+WGAMEH/2+2+i,WGAMEX+WGAMEW/2+2);
		addch(al->action->label[al->action->c]); attroff(A_UNDERLINE);
		i++; al2=al;}	//TODO use al2 len to box the notices properly
	attron(COLOR_PAIR(CP_BASE));
	move(WGAMEY+WGAMEH/2+2+i,WGAMEX+WGAMEW/2+2);
	for (int i=0;i<al2->action->labellen;i++) addch('-');}
return;}
