#include "serenity.h"

int run_game(Game* game, Ui* ui, Ref* ref, Player* pl, World* world){
Map *map =world->curr;
char c=0; do { switch (c){
case K_UP:
case K_DOWN:
case K_LEFT:
case K_RIGHT:	map =movement(pl, world, map, c);	break;
default:	act(ref, map, pl, c);	break;}
display(ui, pl, map); ///!\ here the list of instances is searched twice in the loop
} while((c=getch())!=K_QUIT);	return 0;}



Map* movement(Player* pl, World* world, Map* map, char c){
v2i mv ={0,0}; switch (c){ //change this to generate a mv vector and interpret it after switch loop
case K_UP:	mv.y =-1; mv.x =0;	break;
case K_DOWN:	mv.y =1;  mv.x =0;	break;
case K_LEFT:	mv.y =0;  mv.x =-1;	break;
case K_RIGHT:	mv.y =0;  mv.x =1;	break;
default:				break;}
Tp* tp =NULL; int it =check_collision(map, pl->y+mv.y, pl->x+mv.x, &tp);
if (it>=0){ pl->y+=mv.y; pl->x+=mv.x;}
if (it>0 &&it<'i' &&tp){
	if (!(tp->dstmap)){
		Map* map2 =create_further_map();
		List *new =list_new(t_map, NULL, map2);
		list_insert_before(&(world->maplist), new);
		world->curr =map2;
		pl->y =0; pl->x =0;
		return map2;}
	else{	world->curr =map->tp->dstmap;
		pl->y =0; pl->x =0;
		return map->tp->dstmap;}
}
return map;}


int check_collision(Map* map, int y, int x, Tp** tp){
if (y<0 ||x<0 ||y>=map->h ||x>=map->w)	return -1;
List* inst =list_inst_find(map->inst, y, x);
if (inst){
	char c =((Inter*)(inst->item))->info[y-((Inst*)(inst->inst))->y]
					[x-((Inst*)(inst->inst))->x];
	if (c=='X')	return -1;
	c =((Inter*)(inst->item))->inter[y-((Inst*)(inst->inst))->y]
					[x-((Inst*)(inst->inst))->x];
	if (c=='a'){ *tp =((Inst*)(inst->inst))->tp; return 'a';}}
return 0;}
