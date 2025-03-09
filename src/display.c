#include "serenity.h"

void display_gui(WINDOW* guiwin, Player* pl, Map* map){
wattron(guiwin,COLOR_PAIR(CP_BASE));
for (int i=0; i<GWIN_W; i++) waddch(guiwin,' ');
mvwprintw(guiwin,0,GWIN_W-20,"location: %s",map->name);
for (int i=strlen(map->name); i<10; i++) waddch(guiwin,' ');
wattron(guiwin,COLOR_PAIR(CP_NORMAL));
mvwprintw(guiwin,0,0,"HP        50/50");}


void display_map(WINDOW* gwin, Map* map, v2i pos){
v2i cam   =(v2i){pos.y-GWIN_H/2,pos.x-GWIN_W/2};
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

for (Instance* in=map->inst; in; in=in->next)
if     (in->y+in->inter->h>cam.y &&in->y<cam.y+GWIN_H
      &&in->x+in->inter->w>cam.x &&in->x<cam.x+GWIN_W){
int yy =in->y<cam.y? cam.y-in->y :0;
for (yy; yy<in->inter->h &&in->y+yy<cam.y+GWIN_H; yy++){
	int xx =in->x<cam.x? cam.x-in->x :0;
	for (xx; xx<in->inter->w &&in->x+xx<cam.x+GWIN_W; xx++){
		wmove(gwin, GWIN_H-(cam.y+GWIN_H-in->y)+yy,
			    GWIN_W-(cam.x+GWIN_W-in->x)+xx);
		if (in->inter->map[yy][xx]!=' ')
			waddch(gwin,in->inter->map[yy][xx]);}}}}


void display_pl(WINDOW* gwin,Player* pl, Map* map){
if (map->fg[pl->y][pl->x]) return;
Instance* in =check_inst((v2i){pl->y,pl->x},map);
if (in &&in->inter->info[pl->y-in->y][pl->x-in->x]=='f') return;
wmove(gwin,GWIN_H/2,GWIN_W/2); //TODO exact coordinates
wattron(gwin,COLOR_PAIR(CP_NORMAL));
waddch(gwin,' ');}


void display_notice(WINDOW* gwin, Instance* in, v2i pos, Map* map, int interface_style){
if (interface_style == OLDSCHOOL){
	wattron(gwin,COLOR_PAIR(CP_BASE));
	wmove(gwin,GWIN_H/2+1,GWIN_W/2+1);
	for (int i=0;i<in->inter->actionlist->action->labellen+2;i++)
		waddch(gwin,'-');
	int i=0; Actionlist *al=in->inter->actionlist, *al2;
	for (al;al;al=al->next){
		wmove(gwin,GWIN_H/2+2+i,GWIN_W/2+1);
		waddch(gwin,'|'); wprintw(gwin,"%s",al->action->label); waddch(gwin,'|'); 
		if ((al2 =al->previous)){
			int labellen =al->action->labellen,
			    labellenp =al2->action->labellen;
			if (labellenp>labellen)
				for (int j=0; j<labellenp-labellen; j++)
					waddch(gwin,'-');}
		if ((al2 =al->next)){
			int labellen =al->action->labellen,
			    labellenn =al2->action->labellen;
			if (labellenn>labellen)
				for (int j; j<labellenn-labellen; j++)
					waddch(gwin,'-');}
		wattron(gwin,A_UNDERLINE);
			wmove(gwin,GWIN_H/2+2+i,GWIN_W/2+2);
			waddch(gwin,al->action->label[al->action->c]);
		wattroff(gwin,A_UNDERLINE);
		i++; al2=al;}
	wmove(gwin,GWIN_H/2+2+i,GWIN_W/2+1);
	for (int i=0;i<al2->action->labellen+2;i++) waddch(gwin,'-');}
else if (interface_style == MODERN){
	wattron(gwin,COLOR_PAIR(CP_NORMAL));
	int i=0; for (Actionlist *al=in->inter->actionlist;al;al=al->next){ i++;
		mvwprintw(gwin,GWIN_H/2+i,GWIN_W/2+1,"%s",al->action->label);
	wattron(gwin,A_UNDERLINE);
		wmove(gwin,GWIN_H/2+i,GWIN_W/2+1+al->action->c);
		waddch(gwin,al->action->label[al->action->c]);
	wattroff(gwin,A_UNDERLINE);}}}
