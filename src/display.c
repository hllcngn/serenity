#include "serenity.h"

void display(Ui* ui, Player* pl, Map* map){
display_map(ui->gamw, map, pl->y, pl->x);
Instance* in =get_inst(map, pl->y, pl->x);
display_pl(ui->gamw, pl, map, in);
if (in &&in->inter->inter[pl->y-in->y][pl->x-in->x]=='i')
	display_notice(ui->gamw,pl,map,in,ui->style);
wrefresh(ui->gamw);
display_gui(ui->guiw,pl,map);
wrefresh(ui->guiw);}


void display_gui(WINDOW* guiwin, Player* pl, Map* map){
wattron(guiwin,COLOR_PAIR(CP_BASE));
for (int i=0; i<GWIN_W; i++) waddch(guiwin,' ');
mvwprintw(guiwin,0,GWIN_W-20,"location: %s",map->name);
for (int i=strlen(map->name); i<10; i++) waddch(guiwin,' ');//erase guiwin on location change?
wattron(guiwin,COLOR_PAIR(CP_NORMAL));
mvwprintw(guiwin,0,0,"HP        50/50");} //TODO display actual player hp


void display_pl(WINDOW* gwin, Player* pl, Map* map, Instance* in){
if (map->fg[pl->y][pl->x]) return;
if (in &&in->inter->info[pl->y-in->y][pl->x-in->x]=='f') return;
wattron(gwin,COLOR_PAIR(CP_NORMAL));
mvwaddch(gwin, GWIN_H/2, GWIN_W/2, ' ');}	//bug: when instances overlap
						//-> recover a list of instances at y,x
						//or draw the player first maybe
						// meaning splitting map drawing
						// between background and instances

void display_map(WINDOW* gwin, Map* map, int ply, int plx){
v2i cam   =(v2i){ply-GWIN_H/2,plx-GWIN_W/2};
wattron(gwin,COLOR_PAIR(CP_BASE)); int y=0;
for (; cam.y+y<0; y++){
	wmove(gwin,y,0);
	for (int x=0; x<GWIN_W; x++) waddch(gwin,' ');}
for (; cam.y+y<map->h &&y<GWIN_H; y++){ int x=0;
	wmove(gwin,y,0);
	for (; cam.x+x<0; x++) waddch(gwin,' ');
	for (; cam.x+x<map->w &&x<GWIN_W; x++){ int c;
		if	((c=map->fg[cam.y+y][cam.x+x])) waddch(gwin,c);
		else if	((c=map->clsn[cam.y+y][cam.x+x])) waddch(gwin,c);
		else	waddch(gwin,map->bg[cam.y+y][cam.x+x]);}
	for (; x<GWIN_W; x++) waddch(gwin,' ');}
for (; y<GWIN_H; y++){
	wmove(gwin,y,0);
	for (int x=0;x<GWIN_W;x++) waddch(gwin,' ');}
//TODO find a way to not have to go through the whole list of instances
for (Instance* in=map->inst; in; in=in->next){
v4i inst =(v4i){in->y, in->x, in->inter->h, in->inter->w};
if (inst.y+inst.h>cam.y &&inst.y<cam.y+GWIN_H
  &&inst.x+inst.w>cam.x &&inst.x<cam.x+GWIN_W){
	int y =inst.y<cam.y? cam.y-inst.y :0;
	for (y; y<inst.h &&inst.y+y<cam.y+GWIN_H; y++){
		int x =inst.x<cam.x? cam.x-inst.x :0;
		for (x; x<inst.w &&inst.x+x<cam.x+GWIN_W; x++){
			if (in->type==LOADED &&in->inter->info[y][x]!=' ')
				mvwaddch(gwin, inst.y+y-cam.y, inst.x+x-cam.x,
					in->inter->ascii[y][x]);
			else if (in->type==GENERATED &&in->ascii->info[y][x]!=' ')
				mvwaddch(gwin, inst.y+y-cam.y, inst.x+x-cam.x,
					in->ascii->ascii[y][x]);}}}}}


void display_notice(WINDOW* gwin, Player* pl, Map* map, Instance* in, int interface_style){
Actionlist* aldisp =generate_complete_al(pl, in);
wattron(gwin,COLOR_PAIR(CP_NORMAL));
int i=0; for (Actionlist* al=aldisp; al; al=al->next){
	i++;
	mvwprintw(gwin,GWIN_H/2+i,GWIN_W/2+1,"%s",al->action->label);
	wattron(gwin,A_UNDERLINE);
	wmove(gwin,GWIN_H/2+i,GWIN_W/2+1+al->action->c);
	waddch(gwin,al->action->label[al->action->c]);
	wattroff(gwin,A_UNDERLINE);}
free_actionlist(aldisp);}
