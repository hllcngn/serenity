#include "serenity.h"

Player* create_player(char* name, int y, int x){
Player* pl =malloc(sizeof(Player));
pl->name =name;
pl->y =y; pl->x =x;	return pl;}

void free_player(Player* pl){
free(pl->name); free(pl);	return;}
