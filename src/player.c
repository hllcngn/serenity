#include "serenity.h"

Player* create_player(Ref* ref, char* name, int y, int x, int hp){
Player* pl =malloc(sizeof(Player));
pl->name =name;
pl->y =y; pl->x =x;
pl->hp =hp;
//pl->inventory =NULL;

pl->actlist =NULL;
list_act_insert_new(&(pl->actlist), ref->action[fall_tree], ABLE);
list_act_insert_new(&(pl->actlist), ref->action[harvest_fruits], ABLE);
list_act_insert_new(&(pl->actlist), ref->action[light_fire], SUPERABLE);
return pl;}

void free_player(Player* pl){
free(pl->name);
list_free(pl->actlist);
free(pl);}
