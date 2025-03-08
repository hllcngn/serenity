#include "serenity.h"

int game(vect3f hue, Map* map, Player* pl, Info *info, int interface_style){
vect2i gwin_pos ={(LINES-GWIN_H)/2,(COLS-GWIN_W)/2};
WINDOW* gwin =newwin(GWIN_H,GWIN_W,gwin_pos.y,gwin_pos.x);
WINDOW* guiwin =newwin(1,GWIN_W,gwin_pos.y-2,gwin_pos.x);
Map *newmap,*oldmap; newmap=oldmap=map;
char c=0; Instance* inst; do { switch (c){
case K_UP:
case K_DOWN:
case K_LEFT:
case K_RIGHT:	newmap=movement(c,pl,map);
		if (newmap!=map){
			map=newmap;
			pl->x =2; pl->y =2;}
		break;
default:
	inst =check_inst((vect2i){pl->y,pl->x},map);
	if (inst &&inst->inter->inter[pl->y-inst->y][pl->x-inst->x]=='i'){
		Actionlist* al;
		 for (al=inst->inter->actionlist;
		   al &&al->action->k!=c;
		   al=al->next);
		 if (al) al->action->action(inst,map,info);}	break;}

display_map(gwin, map, (vect2i){pl->y,pl->x});
display_pl(gwin, pl, map);
Instance* in =check_inst((vect2i){pl->y,pl->x},map);
if (in &&in->inter->inter[pl->y-in->y][pl->x-in->x]=='i')
	display_notice(gwin,in,(vect2i){pl->y,pl->x},map,interface_style);
wrefresh(gwin);
display_gui(guiwin,pl,map);
wrefresh(guiwin);
} while((c=getch())!=K_QUIT);
delwin(gwin); return 0;}



Map* movement(char c, Player* pl, Map* map){
Map* newmap=map;
int tp; switch (c){
case K_UP:    if(!check_collision((vect2i){pl->y-1,pl->x}, map)){
			pl->y--;
			if(check_tp((vect2i){pl->y,pl->x},map))
				newmap =load_map(map->house);
	      } break;
case K_DOWN:  if(!check_collision((vect2i){pl->y+1,pl->x}, map))
			pl->y++;	break;
case K_LEFT:  if(!check_collision((vect2i){pl->y,pl->x-1}, map))
			pl->x--;	break;
case K_RIGHT: if(!check_collision((vect2i){pl->y,pl->x+1}, map))
			pl->x++;	break;
default:				break;}
return newmap;}

int check_collision(vect2i pos, Map* map){
if (pos.y>=0	  &&pos.x>=0
  &&pos.y<map->h  &&pos.x<map->w
  &&!map->clsn[pos.y][pos.x]){
	Instance* it =check_inst(pos,map);
	if (it &&it->inter->info[pos.y-it->y][pos.x-it->x]=='X')
		return 1;
	return 0;}
return 1;}

int check_tp(vect2i pos, Map* map){
if (pos.y>=0	  &&pos.x>=0
  &&pos.y<map->h  &&pos.x<map->w
  &&!map->clsn[pos.y][pos.x]){
	if (map->tp[pos.y][pos.x])
		return map->tp[pos.y][pos.x];
	return 0;}
return 0;}
