#include "serenity.h"

Player* create_player(char* name, int y, int x, int hp){
Player* pl =malloc(sizeof(Player));
pl->name =name;
pl->y =y; pl->x =x;
pl->hp =hp;

pl->actions =malloc(sizeof(AID)*nb_action);
pl->actions[fall_tree]=1;
pl->actions[harvest_fruits]=1;
pl->actions[pull_stump]=0;
pl->actions[light_fire]=0;
return pl;}

void free_player(Player* pl){
free(pl->name);
free(pl->actions);
free(pl);}
