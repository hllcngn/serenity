#include "serenity.h"

void display(Ui *ui, Player *pl, Map *map){
List* inst= list_inst_find(map->inst, pl->y, pl->x);
display_map_bg(ui->gamw, map, pl->y, pl->x);
display_pl(ui->gamw, pl, map, inst);
display_map_inst(ui->gamw, map, pl->y, pl->x);
display_notice(ui->gamw, pl, map, inst);
wrefresh(ui->gamw);

display_gui(ui->guiw,pl,map);
wrefresh(ui->guiw);}



void display_map_bg(WINDOW *gwin, Map *map, int ply, int plx){
v2i cam= (v2i){ply -GWIN_H/2, plx -GWIN_W/2};
wattron(gwin, COLOR_PAIR(CP_BASE)); int y=0;
for (; cam.y+y<0; y++){
	wmove(gwin,y,0);
	for (int x=0; x<GWIN_W; x++) waddch(gwin,' ');}
for (; cam.y+y<map->h &&y<GWIN_H; y++){ int x=0;
	wmove(gwin,y,0);
	for (; cam.x+x<0; x++) waddch(gwin,' ');
	for (; cam.x+x<map->w &&x<GWIN_W; x++){ int c;
		waddch(gwin,map->bg[cam.y+y][cam.x+x]);}
	for (; x<GWIN_W; x++) waddch(gwin,' ');}
for (; y<GWIN_H; y++){
	wmove(gwin,y,0);
	for (int x=0;x<GWIN_W;x++) waddch(gwin,' ');}}


void display_map_inst(WINDOW *gwin, Map *map, int ply, int plx){
v2i cam= (v2i){ply -GWIN_H/2, plx -GWIN_W/2};
wattron(gwin, COLOR_PAIR(CP_BASE));
//TODO find a way to not have to go through the whole list of instances
for (List* i=map->inst; i; i=i->next){
v4i inst =(v4i){((Inst*)(i->inst))->y,
		((Inst*)(i->inst))->x,
		((Inter*)(i->item))->h,
		((Inter*)(i->item))->w};
if (inst.y+inst.h>cam.y &&inst.y<cam.y+GWIN_H
  &&inst.x+inst.w>cam.x &&inst.x<cam.x+GWIN_W){
	int y =inst.y<cam.y? cam.y-inst.y :0;
	for (y; y<inst.h &&inst.y+y<cam.y+GWIN_H; y++){
		int x =inst.x<cam.x? cam.x-inst.x :0;
		for (x; x<inst.w &&inst.x+x<cam.x+GWIN_W; x++)
			if (((Inter*)(i->item))->info[y][x]!=' ')
				mvwaddch(gwin, inst.y+y-cam.y, inst.x+x-cam.x,
					((Inter*)(i->item))->ascii[y][x]);}}}}


void display_pl(WINDOW *gwin, Player *pl, Map *map, List *inst){
wattron(gwin,COLOR_PAIR(CP_NORMAL));
mvwaddch(gwin, GWIN_H/2, GWIN_W/2, ' ');}


void display_notice(WINDOW *gwin, Player *pl, Map *map, List *inst){
if (!inst)	return;
if (!(((Inter*)(inst->item))->inter[pl->y -((Inst*)(inst->inst))->y]
				[pl->x -((Inst*)(inst->inst))->x]=='i')) return;
List* list= list_act_generate(pl, inst);
wattron(gwin, COLOR_PAIR(CP_NORMAL));
int i=0; for (List* l=list; l; l=l->next){ i++;
	mvwprintw(gwin, GWIN_H/2+i, GWIN_W/2+1, "%s", ((Action*)(l->item))->label);
	wattron(gwin, A_UNDERLINE);
	wmove(gwin, GWIN_H/2+i, GWIN_W/2+1+((Action*)(l->item))->c);
	waddch(gwin, ((Action*)(l->item))->label[((Action*)(l->item))->c]);
	wattroff(gwin, A_UNDERLINE);}
list_free(list);}



void display_gui(WINDOW *guiwin, Player *pl, Map *map){
wattron(guiwin, COLOR_PAIR(CP_BASE));
for (int i=0; i<GWIN_W; i++) waddch(guiwin, ' ');
if (map->name) mvwprintw(guiwin,0, GWIN_W-20, "location: %s", map->name);
for (int i=strlen(map->name); i<10; i++) waddch(guiwin, ' ');
wattron(guiwin, COLOR_PAIR(CP_NORMAL));
mvwprintw(guiwin, 0, 0, "HP        50/50");} //TODO actual player hp
