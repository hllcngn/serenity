#include "serenity.h"

Player* create_player(Ref* ref, char* name, int y, int x, int hp){
Player* pl =malloc(sizeof(Player));
pl->name =name;
pl->y =y; pl->x =x;
pl->hp =hp;

pl->actionlist =NULL;
add_action(&(pl->actionlist), ref->action[fall_tree], ABLE);
add_action(&(pl->actionlist), ref->action[harvest_fruits], ABLE);
return pl;}

void free_player(Player* pl){
free(pl->name);
//free(pl->actions);
free_actionlist(pl->actionlist);
free(pl);}
