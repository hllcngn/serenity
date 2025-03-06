#include "serenity.h"

void display_map(Map* map, vect2i pos){
vect2i wgame =(vect2i){(LINES-WGAMEH)/2,(COLS-WGAMEW)/2};//why isn't this in a window for simplicity
vect2i cam   =(vect2i){pos.y-WGAMEH/2,  pos.x-WGAMEW/2};
attron(COLOR_PAIR(CP_NORMAL));
mvprintw(wgame.y-2,wgame.x,"HP        50/50");
int y=0; attron(COLOR_PAIR(CP_BASE));
mvprintw(wgame.y-2,wgame.x+WGAMEW-20,"location: %s",map->name);
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
			addch(in->inter->map[yy][xx]);}}}}


void display_pl(Player* pl, Map* map){
if (map->fg[pl->y][pl->x]) return;
Instance* in =check_inst((vect2i){pl->y,pl->x},map);
if (in &&in->inter->info[pl->y-in->y][pl->x-in->x]=='f') return;
vect2i wgame =(vect2i){(LINES-WGAMEH)/2,(COLS-WGAMEW)/2};
move(wgame.y+WGAMEH/2,wgame.x+WGAMEW/2); //TODO exact coordinates
attron(COLOR_PAIR(CP_NORMAL)); addch(' ');	return;}


void display_notice(vect2i pos, Map* map, int interface_style){
Instance* in =check_inst(pos,map);
if (in &&in->inter->inter[pos.y-in->y][pos.x-in->x]=='i'){
	attron(COLOR_PAIR(CP_BASE));
	vect2i wgame =(vect2i){(LINES-WGAMEH)/2,(COLS-WGAMEW)/2};
	//move(wgame.y+WGAMEH/2+1,wgame.x+WGAMEW/2+2);
	if (interface_style == OLDSCHOOL){
		move(wgame.y+WGAMEH/2+1,wgame.x+WGAMEW/2+1);
		for (int i=0;i<in->inter->actionlist->action->labellen+2;i++)
			addch('-');
		int i=0; Actionlist *al=in->inter->actionlist, *al2;
		for (al;al;al=al->next){
			move(wgame.y+WGAMEH/2+2+i,wgame.x+WGAMEW/2+1);
			addch('|'); printw("%s",al->action->label); addch('|'); 
			if ((al2 =al->previous)){
				int labellen =al->action->labellen,
				    labellenp =al2->action->labellen;
				if (labellenp>labellen)
					for (int j=0; j<labellenp-labellen; j++)
						addch('-');}
			if ((al2 =al->next)){
				int labellen =al->action->labellen,
				    labellenn =al2->action->labellen;
				if (labellenn>labellen)
					for (int j; j<labellenn-labellen; j++)
						addch('-');}
			attron(A_UNDERLINE);
				move(wgame.y+WGAMEH/2+2+i,wgame.x+WGAMEW/2+2);
				addch(al->action->label[al->action->c]);
			attroff(A_UNDERLINE);
			i++; al2=al;}
		move(wgame.y+WGAMEH/2+2+i,wgame.x+WGAMEW/2+1);
		for (int i=0;i<al2->action->labellen+2;i++) addch('-');}
	else {
		attron(COLOR_PAIR(CP_NORMAL));
		int i=0; for (Actionlist *al=in->inter->actionlist;al;al=al->next){ i++;
			mvprintw(wgame.y+WGAMEH/2+i,wgame.x+WGAMEW/2+1,"%s",al->action->label);
		attron(A_UNDERLINE);
			move(wgame.y+WGAMEH/2+i,wgame.x+WGAMEW/2+1);
			addch(al->action->label[al->action->c]);
		attroff(A_UNDERLINE);}
	}
}}
