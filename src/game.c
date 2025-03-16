#include "serenity.h"

int game(v3f hue, Map* map, Player* pl, Ref* ref, Ui* ui){
Map *newmap,*oldmap; newmap=oldmap=map;
char c=0; Instance* inst; do { switch (c){
case K_UP:
case K_DOWN:
case K_LEFT:
case K_RIGHT:	newmap=movement(c,pl,map,oldmap);
		if (newmap!=map){
			if (newmap->type ==OUTDOORS){
				pl->y =newmap->house->y+pl->y;
				pl->x =newmap->house->x+pl->x;}
			else if (newmap->type ==INDOORS){
				pl->y =pl->y-map->house->y;
				pl->x =pl->x-map->house->x;}
			map=newmap;
		} break;
default:	act(ref, map, pl, c);	break;
}

display_map(ui->gamw, map, (v2i){pl->y,pl->x});
display_pl(ui->gamw, pl, map);
Instance* in =check_inst((v2i){pl->y,pl->x},map);
if (in &&in->inter->inter[pl->y-in->y][pl->x-in->x]=='i')
	display_notice(ui->gamw,pl,in,(v2i){pl->y,pl->x},map,ui->style);
wrefresh(ui->gamw);
display_gui(ui->guiw,pl,map);
wrefresh(ui->guiw);
} while((c=getch())!=K_QUIT);	return 0;}



Map* movement(char c, Player* pl, Map* map, Map* oldmap){
Map* newmap=map;
int tp; switch (c){
case K_UP:    if(!check_collision((v2i){pl->y-1,pl->x}, map)){
			pl->y--;
			if(check_tp((v2i){pl->y,pl->x},map)){
				save_map(map);
				if (map->type==OUTDOORS)
					newmap =load_map(map->house,map);
				else	newmap =oldmap;}
	      } break;
case K_DOWN:    if(!check_collision((v2i){pl->y+1,pl->x}, map)){
			pl->y++;
			if(check_tp((v2i){pl->y,pl->x},map)){
				save_map(map);
				if (map->type==OUTDOORS)
					newmap =load_map(map->house,map);
				else	newmap =oldmap;}
	      } break;
case K_LEFT:  if(!check_collision((v2i){pl->y,pl->x-1}, map))
			pl->x--;	break;
case K_RIGHT: if(!check_collision((v2i){pl->y,pl->x+1}, map))
			pl->x++;	break;
default:				break;}
return newmap;}

int check_collision(v2i pos, Map* map){
if (pos.y>=0	  &&pos.x>=0
  &&pos.y<map->h  &&pos.x<map->w
  &&!map->clsn[pos.y][pos.x]){
	Instance* it =check_inst(pos,map);
	if (it &&it->inter->info[pos.y-it->y][pos.x-it->x]=='X')
		return 1;
	return 0;}
return 1;}

int check_tp(v2i pos, Map* map){
if (map->tp[pos.y][pos.x])
	return map->tp[pos.y][pos.x];
return 0;}
